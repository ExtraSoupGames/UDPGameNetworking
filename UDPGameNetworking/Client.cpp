#include "Client.h"

Client::Client(int portToUse)
{
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
	delete serverAddress;
	SDLNet_DestroyDatagramSocket(socket);
	delete socket;
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

ImportantMessage* Client::ProcessImportantMessage(NetworkMessage* msg)
{
	if (!IsConnected()) return nullptr;
	return sender->ProcessImportantMessage(msg);
}