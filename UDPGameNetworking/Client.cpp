#include "Client.h"
#include "Wrapper/IWrapper.h"
void Client::PollSocket()
{
	NetworkMessage* nextMessage;
	while (NetworkUtilities::GetNextIncoming(socket, nextMessage, sender)) {
		ProcessMessage(nextMessage);
		delete nextMessage;
	}
}

void Client::ProcessMessage(NetworkMessage* msg)
{
	std::string msgData = msg->GetExtraData();
	switch (msg->GetMessageType()) {
	case UserImportant:
		ProcessImportantMessage(msg);
		//no break here as the message should still be processed as a user message as well
	case UserUnImportant:
		ProcessUserMessage();
		break;
	case IDRequest:
		ProcessIncomingIDRequest(msg);
		break;
	case NetworkedObjectMsg:
		break;
	case ImportantMessageConfirmation:
		break;
	}
}

ImportantMessage* Client::ProcessImportantMessage(NetworkMessage* msg)
{
	if (!IsConnected()) return nullptr;
	return sender->ProcessImportantMessage(msg);
}

void Client::ProcessIncomingIDRequest(NetworkMessage* msg)
{
	int ID = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(0, objectIDBits),objectIDDigits);
	for (OwnedNetworkObject* ono : *ownedObjects) {
		if (ono->IDRequestReceived(ID)) {
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
	for (UnownedNetworkObject* uno : *nonOwnedObjects) {
		if (uno->StreamDataReceived(msg)) {
			return;
		}
	}
}

void Client::ProcessImportantMessageConfirmation(NetworkMessage* msg)
{
	sender->ConfirmationRecieved(msg);
}

Client::Client(int portToUse, IWrapper* libWrapper)
{
	wrapper = libWrapper;

	port = portToUse;
	sender = nullptr;
	socket = nullptr;
	serverAddress = nullptr;

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
	SDLNet_UnrefAddress(serverAddress);
	SDLNet_DestroyDatagramSocket(socket);
}

void Client::ConnectToServer(std::string address)
{
	if (serverAddress != nullptr) {
		std::cout << "Already connected to a server!" << std::endl;
	}
	serverAddress = SDLNet_ResolveHostname(address.c_str());
	SDLNet_WaitUntilResolved(serverAddress, 1000);
	socket = SDLNet_CreateDatagramSocket(serverAddress, port);

	if (!socket) {
		std::cout << "Invalid socket" << SDL_GetError() << std::endl;
	}

	sender = new ClientMessageSender(socket, serverAddress, 66661);
}

void Client::Disconnect()
{
	delete sender;
	sender = nullptr;
	SDLNet_UnrefAddress(serverAddress);
	serverAddress = nullptr;
	SDLNet_DestroyDatagramSocket(socket);
	socket = nullptr;
}

void Client::Update(float deltaTime)
{
	if (IsConnected()) {

		sender->SendUnsentMessages(false);
		NetworkMessage* msg = nullptr;
		while (NetworkUtilities::GetNextIncoming(socket, msg, sender)) {
			//TODO user managed callbacks on messages received
			//TODO library managed non owned objects updated and wrapper notified
			delete msg;
			msg = nullptr;
		}
	}
}

bool Client::IsConnected()
{
	if (serverAddress == nullptr) return false;
	if (socket == nullptr) return false;
	if (sender == nullptr) return false;
	return true;
}

void Client::SendServerMessage(NetworkMessageTypes type, std::string msg, std::string stateCode)
{
	if (!IsConnected()) return;
	NetworkUtilities::SendMessageTo(type, msg, socket, serverAddress, 66661, sender);
}
