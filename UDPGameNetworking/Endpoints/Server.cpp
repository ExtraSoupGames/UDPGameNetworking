#include "Server.h"
#include "../Wrapper/IWrapper.h"
int Server::GetNextFreeID()
{
    int out = availableIDs->front();
    availableIDs->pop();
    if (availableIDs->size() == 0) {
        availableIDs->push(out + 1);
    }
    std::cout << "Next free id was: " << out << std::endl;
    return out;
}
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

void Server::PollSocket()
{
    NetworkMessage* nextMessage = nullptr;
    while (NetworkUtilities::GetNextIncoming(socket, nextMessage, sender)) {
        ProcessMessage(nextMessage);
        delete nextMessage;
    }
}

void Server::ProcessMessage(NetworkMessage* msg)
{
    switch (msg->GetMessageType()) {
    case UserImportant:
        msg = sender->ProcessImportantMessage(msg);
        if (msg == nullptr) { return; }
    case UserUnImportant:
        ProcessUserMessage(msg);
        break;
    case IDRequest:
        ProcessIncomingIDRequest(msg);
        break;
    case NetworkedObjectMsg:
        ProcessObjectMessage(msg);
        break;
    }
}

void Server::ProcessIncomingIDRequest(NetworkMessage* msg)
{
    std::string idValue = NetworkUtilities::AsBinaryString(GetNextFreeID(), objectIDDigits);
    NetworkUtilities::SendMessageTo(IDRequest, idValue, socket, msg->GetAddress(), msg->GetPort(), sender);
}

void Server::ProcessUserMessage(NetworkMessage* msg)
{
    //TODO implement
    std::cout << "User message received by server!" << std::endl;
}

void Server::ProcessObjectMessage(NetworkMessage* msg)
{
    //TODO implement
    //TODO remove this - just for testing
    int objectID = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(0, 8), objectIDDigits);
    int objectX = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(16,28), 7);
    int objectY = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(44, 28), 7);
    if (objectX == objectY && objectY == objectID) {
        std::cout << "YAY";
    }
    else {
        std::cout << "UH OH!!!!!" << std::endl;
    }
    
}


Server::Server(std::string ip, int serverPort, IWrapper* libWrapper)
{
    wrapper = libWrapper;
    connectingAClient = false;
    connectorInfo = nullptr;
    nextClientID = 0;
    port = serverPort;

    ownedObjects = new std::vector<OwnedNetworkObject*>();
    nonOwnedObjects = new std::vector<UnownedNetworkObject*>();
    availableIDs = new std::queue<int>();
    availableIDs->push(1);
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
    PollSocket();
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


Server::~Server()
{
	delete sender;
	SDLNet_DestroyDatagramSocket(socket);
	for (EndpointInfo* client : *connectedClients) {
		delete client;
	}
    delete connectedClients;
    delete availableIDs;
}
