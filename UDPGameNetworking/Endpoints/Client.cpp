#include "Client.h"
#include "../Wrapper/IWrapper.h"

void Client::ProcessMessage(NetworkMessage* msg)
{
	switch (msg->GetMessageType()) {
	case UserImportant:
		ProcessUserMessage(msg);
		return;
	case UserUnImportant:
		ProcessUserMessage(msg);
		return;
	case IDRequest:
		ProcessIncomingIDRequest(msg);
		return;
	case NetworkedObjectInit:
		InitializeNewObject(msg);
		return;
	case NetworkedObjectMsg:
		ProcessObjectMessage(msg);
		return;
	case ImportantMessageConfirmation:
		sender->ConfirmationRecieved(msg);
		return;
	case Connect:
		NetworkUtilities::SendMessageTo(ConnectConfirm, "", socket, serverInfo->address, serverInfo->port, sender);
		std::cout << "Connect confirmation sent!" << std::endl;
		isConnected = true;
		return;
	}
}


void Client::ProcessIncomingIDRequest(NetworkMessage* msg)
{
	int ID = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(0, objectIDBits), objectIDDigits);
	for (OwnedNetworkObject* ono : *ownedObjects) {
		if (ono->IDRequestReceived(ID, socket, serverInfo, sender, wrapper)) {
			std::cout << "Object with ID " << ID << " registered successfully!" << std::endl;
			return;
		}
	}
}

void Client::ProcessUserMessage(NetworkMessage* msg)
{
	//TODO test extra data parsing
	std::string msgData = msg->GetExtraData();
	int callbackID = NetworkUtilities::IntFromBinaryString(msgData.substr(0, 12), 3);
	std::string optionalExtraData = msgData.substr(12);
	wrapper->InvokeRegisteredCallback(callbackID, optionalExtraData);
}

void Client::ProcessObjectMessage(NetworkMessage* msg)
{
	//TODO move this to a better location so server/client sync isnt dependent on networked objects being initialized (library should be able to work with no networked object and only user messages if desired)
	if (!TimerStarted()) {
		int messageTimestamp = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(8, timestampByteCount * 8), timestampByteCount * 2);
		StartTimer(messageTimestamp);
	}
	//first we should check if the object is owned by this client and if so, ignore the message
	int streamObjectID = NetworkUtilities::GetObjectIDFromMsg(msg);
	if (AmIThisObjectsOwner(streamObjectID)) {
		//std::cout << "Client received object data but it was ignored as the clients was the objects owner" << std::endl;
		return;
	}
	for (UnownedNetworkObject* uno : *nonOwnedObjects) {
		if (uno->StreamDataReceived(msg, settings)) {
			//if the object exists we can return
			return;
		}
	}
	//if the object was not found then it was not initialized properly
	std::cout << "Object not initialized properly! object not found in initialized objects, but data received" << std::endl;

}

void Client::InitializeNewObject(NetworkMessage* msg)
{
	//TODO fix type being 0 always
	int objectType = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(objectIDBits, 8), 2);
	IEngineObject* engineObj = wrapper->NewNetworkedObject(objectType, true);
	UnownedNetworkObject* uno = new UnownedNetworkObject(engineObj, msg, wrapper);
	nonOwnedObjects->push_back(uno);
}

void Client::UpdateObjects(float deltaTime)
{
	for (OwnedNetworkObject* ono : *ownedObjects) {
		ono->Update(deltaTime, serverInfo, socket, GetTime(), settings);
	}
}

bool Client::AmIThisObjectsOwner(int objectID)
{
	for (OwnedNetworkObject* ono : *ownedObjects) {
		if (ono->GetID() == objectID) {
			return true;
		}
	}
	return false;
}

void Client::SendConnectRequest()
{
	NetworkUtilities::SendMessageTo(Connect, "", socket, serverInfo->address, serverInfo->port, sender);
}

Client::Client(int portToUse, IWrapper* libWrapper, LibSettings* libSettings)
{
	wrapper = libWrapper;
	settings = libSettings;

	port = portToUse;
	sender = nullptr;
	socket = nullptr;
	serverInfo = nullptr;
	isConnected = false;
	fallbackConnectionRequestDelay = 0;
	connecting = false;

	ownedObjects = new std::vector<OwnedNetworkObject*>();
	nonOwnedObjects = new std::vector<UnownedNetworkObject*>();
}

Client::~Client()
{
	for (auto& o : *ownedObjects) {
		delete o;
	}
	delete ownedObjects;

	for (auto& o : *nonOwnedObjects) {
		delete o;
	}
	delete nonOwnedObjects;

	delete sender;
	delete serverInfo;
	SDLNet_DestroyDatagramSocket(socket);
}

void Client::ConnectToServer(std::string address)
{
	connecting = true;
	if (serverInfo != nullptr) {
		std::cout << "Already connected to a server!" << std::endl;
	}
	serverInfo = new EndpointInfo(address, 55533);
	socket = SDLNet_CreateDatagramSocket(serverInfo->address, port);

	if (!socket) {
		std::cout << "Invalid socket" << SDL_GetError() << std::endl;
	}

	sender = new ClientMessageSender(socket, serverInfo);
	SendConnectRequest();
}

void Client::Disconnect()
{
	delete sender;
	sender = nullptr;
	delete serverInfo;
	SDLNet_DestroyDatagramSocket(socket);
	socket = nullptr;
}

void Client::Update(float deltaTime)
{
	if (connecting && !isConnected) {
		if (fallbackConnectionRequestDelay < 0) {
			SendConnectRequest();
			std::cout << "sending fallback connect request" << std::endl;
			fallbackConnectionRequestDelay = 5000;
		}
		fallbackConnectionRequestDelay--;
	}
	if (IsConnected()) {
		sender->SendUnsentMessages(false);
		PollSocket();
		UpdateObjects(deltaTime);
	}
}

bool Client::IsConnected()
{
	if (serverInfo == nullptr) return false;
	if (socket == nullptr) return false;
	if (sender == nullptr) return false;
	return true;
}

void Client::SendServerMessage(NetworkMessageTypes type, std::string msg)
{
	if (!IsConnected()) return;
	NetworkUtilities::SendMessageTo(type, msg, socket, serverInfo->address, serverInfo->port, sender);
}

void Client::RegisterObject(IEngineObject* obj)
{
	OwnedNetworkObject* ono = new OwnedNetworkObject(serverInfo, socket, obj);
	ownedObjects->push_back(ono);
}
