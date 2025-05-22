#include "Server.h"

void Server::ConfirmClientConnection(EndpointInfo* client)
{
    if (!connectingAClient) {
        return;
    }
    if (SDLNet_GetAddressString(client->address) == SDLNet_GetAddressString(connectorInfo->address)) {
        connectingAClient = false;
        nextClientID++;
        connectedClients->push_back(new EndpointInfo(client->address, client->port));
        sender->NewClientConnected(connectedClients->at(connectedClients->size() - 1));
        sender->SendImportantMessage(Error, "1111", new EndpointInfo(client->address, client->port));
        connectorInfo = nullptr;
        std::cout << "Successfully connected client with ID: " << (nextClientID - 1) << std::endl;
    }
}

void Server::TryConnectClient(std::string inData, EndpointInfo* client)
{
    if (IsAlreadyConnected(client)) {
        return;
    }
    if (connectingAClient) {
        if (*client == *connectorInfo) {

        }
    }
}

bool Server::IsAlreadyConnected(EndpointInfo* client)
{
    for (EndpointInfo* connectedClient : *connectedClients) {
        if (client == connectedClient) {
            return true;
        }
    }
    return false;
}

Server::Server(std::string ip, int serverPort)
{
    connectingAClient = false;
    connectorInfo = nullptr;
    nextClientID = 0;
    port = serverPort;

    ownedObjects = new std::vector<OwnedNetworkObject*>();
    nonOwnedObjects = new std::vector<UnownedNetworkObject*>();
    connectedClients = new std::vector<EndpointInfo*>();


    address = SDLNet_ResolveHostname(ip.c_str());
    //TODO fix blocking until address is resolved
    SDLNet_WaitUntilResolved(address, -1);
    socket = SDLNet_CreateDatagramSocket(address, port);
    if (!socket) {
        printf("Failed to create UDP socket: %s\n", SDL_GetError());
        sender = nullptr;
        return;
    }


    sender = new ServerMessageSender(socket, connectedClients);

}

void Server::Update(float deltaTime)
{
    NetworkMessage* msg = nullptr;
    while (NetworkUtilities::GetNextIncoming(socket, msg, sender)) {
        std::cout << "Done: extra data was" << msg->GetExtraData() << std::endl;
        delete msg;
        msg = nullptr;
    }
}

void Server::Broadcast(std::string message)
{
    for (int i = 0; i < connectedClients->size(); i++) {
        EndpointInfo* c = connectedClients->at(i);
        NetworkUtilities::SendMessageTo(Error, message, socket, c->address, c->port);
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
    return (int)connectedClients->size();
}

void Server::SendMessageTo(NetworkMessageTypes type, std::string message, EndpointInfo* receiver)
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
	SDLNet_DestroyDatagramSocket(socket);
	for (EndpointInfo* client : *connectedClients) {
		delete client;
	}
    delete connectedClients;
}
