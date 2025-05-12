#include "Server.h"

void Server::ConfirmClientConnection(ClientInfo* client)
{
    if (!connectingAClient) {
        return;
    }
    if (SDLNet_GetAddressString(client->address) == SDLNet_GetAddressString(connectorInfo->address)) {
        connectingAClient = false;
        nextClientID++;
        connectedClients->push_back(new ClientInfo(client->address, client->clientPort));
        sender->NewClientConnected(connectedClients->at(connectedClients->size() - 1));
        sender->SendImportantMessage(Error, "1111", new ClientInfo(client->address, client->clientPort));
        connectorInfo = nullptr;
        std::cout << "Successfully connected client with ID: " << (nextClientID - 1) << std::endl;
    }
}

void Server::TryConnectClient(std::string inData, ClientInfo* client)
{
    if (IsAlreadyConnected(client)) {
        return;
    }
    if (connectingAClient) {
        if (*client == *connectorInfo) {

        }
    }
}

bool Server::IsAlreadyConnected(ClientInfo* client)
{
    for (ClientInfo* connectedClient : *connectedClients) {
        if (client == connectedClient) {
            return true;
        }
    }
    return false;
}

Server::Server(std::string ip, int port)
{
    address = SDLNet_ResolveHostname(ip.c_str());
    if (address == nullptr) {
        std::cout << ("Address is invalid!") << std::endl;
        return;
    }
    //TODO fix blocking until address is resolved
    SDLNet_WaitUntilResolved(address, -1);
    socket = SDLNet_CreateDatagramSocket(address, 66661);
    if (!socket) {
        printf("Failed to create UDP socket: %s\n", SDL_GetError());
        return;
    }

    connectingAClient = false;
    connectorInfo = nullptr;
    nextClientID = 0;

    connectedClients = new std::vector<ClientInfo*>();

    sender = new ServerMessageSender(socket, connectedClients);

}

void Server::Update(float deltaTime)
{
    NetworkMessage* msg = nullptr;
    while (NetworkUtilities::GetNextIncoming(socket, msg, sender)) {
        std::cout << "Done: extra data was" << msg->GetExtraData() << std::endl;

    }
}

void Server::Broadcast(std::string message)
{
    for (int i = 0; i < connectedClients->size(); i++) {
        ClientInfo* c = connectedClients->at(i);
        NetworkUtilities::SendMessageTo(Error, message, socket, c->address, c->clientPort);
    }
}

void Server::ImportantBroadcast(NetworkMessageTypes type, std::string message)
{
    if (!sender) {
        std::cout << "Sender is null, not sending important broadcast" << std::endl;
        return;
    }
    sender->BroadcastImportantMessage(type, message);
}

int Server::GetConnectedClientCount()
{
    return connectedClients->size();
}

void Server::SendMessageTo(NetworkMessageTypes type, std::string message, ClientInfo* receiver)
{
    NetworkUtilities::SendMessageTo(type, message, socket, address, port, sender);
}

ImportantMessage* Server::ProcessImportantMessage(NetworkMessage* msg)
{
    return sender->ProcessImportantMessage(msg);
}

Server::~Server()
{
	delete sender;
	delete socket;
	for (ClientInfo* client : *connectedClients) {
		delete client;
	}
    delete connectedClients;
}
