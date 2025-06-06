#include "Client.h"
#include "../Wrapper/IWrapper.h"
void Client::PollSocket()
{
	NetworkMessage* nextMessage = nullptr;
	while (NetworkUtilities::GetNextIncoming(socket, nextMessage, sender)) {
		ProcessMessage(nextMessage);
		delete nextMessage;
	}
}

void Client::ProcessMessage(NetworkMessage* msg)
{
	switch (msg->GetMessageType()) {
	case UserImportant:
		msg = sender->ProcessImportantMessage(msg);
		//this will return if the message has already been received
		if (msg == nullptr) {return;}
		//no break here as the message should still be processed as a user message as well
	case UserUnImportant:
		ProcessUserMessage(msg);
		break;
	case IDRequest:
		ProcessIncomingIDRequest(msg);
		break;
	case NetworkedObjectMsg:
		ProcessObjectMessage(msg);
		break;
	case ImportantMessageConfirmation:
		sender->ConfirmationRecieved(msg);
		break;
	}
}


void Client::ProcessIncomingIDRequest(NetworkMessage* msg)
{
	int ID = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(0, objectIDBits), objectIDDigits);
	std::cout << "ID confirmation received, ID is: " << ID << std::endl;
	std::cout << "size of onos is: " << ownedObjects->size() << std::endl;
	for (OwnedNetworkObject* ono : *ownedObjects) {
		if (ono->IDRequestReceived(ID)) {
			std::cout << "Object with ID " << ID << " registered successfully!" << std::endl;
			return;
		}
	}
}

void Client::ProcessUserMessage(NetworkMessage* msg)
{
	//TODO include optional extra data parsing
	//I.E any data included after the initial 8 bits will be passed to the callback to allow
	// more custom functionality and fewer unique callback registers
	std::string msgData = msg->GetExtraData();
	int callbackID = NetworkUtilities::IntFromBinaryString(msgData.substr(0, 8), 4);
	wrapper->InvokeRegisteredCallback(callbackID);
}

void Client::ProcessObjectMessage(NetworkMessage* msg)
{
	bool relevantObjectExists = false; //does the object described in the message exist in this client's view already
	// if not we need to create it
	for (UnownedNetworkObject* uno : *nonOwnedObjects) {
		if (uno->StreamDataReceived(msg)) {
			relevantObjectExists = true;
			return;
		}
	}
	if (!relevantObjectExists) {
		IEngineObject* engineObj = wrapper->NewNetworkedObject(0);
		UnownedNetworkObject* uno = new UnownedNetworkObject(engineObj);
		nonOwnedObjects->push_back(uno);
	}
}

void Client::UpdateObjects(float deltaTime)
{
	for (OwnedNetworkObject* ono : *ownedObjects) {
		ono->Update(deltaTime, serverInfo, socket);

	}
}

Client::Client(int portToUse, IWrapper* libWrapper)
{
	wrapper = libWrapper;

	port = portToUse;
	sender = nullptr;
	socket = nullptr;
	serverInfo = nullptr;

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
	if (serverInfo != nullptr) {
		std::cout << "Already connected to a server!" << std::endl;
	}
	serverInfo = new EndpointInfo(address, 66661);
	socket = SDLNet_CreateDatagramSocket(serverInfo->address, port);

	if (!socket) {
		std::cout << "Invalid socket" << SDL_GetError() << std::endl;
	}

	sender = new ClientMessageSender(socket, serverInfo);
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

void Client::SendServerMessage(NetworkMessageTypes type, std::string msg, std::string stateCode)
{
	if (!IsConnected()) return;
	NetworkUtilities::SendMessageTo(type, msg, socket, serverInfo->address, serverInfo->port, sender);
}

void Client::RegisterObject(IEngineObject* obj)
{
	OwnedNetworkObject* ono = new OwnedNetworkObject(serverInfo, socket, obj);
	ownedObjects->push_back(ono);
}
