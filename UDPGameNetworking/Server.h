#pragma once
#include "Senders/ServerMessageSender.h"
#include <string>
#include "Wrapper/IWrapper.h"
#include "NetworkedObject.h"
class Server {
private:
	//Extract to UDPEndpoint
	SDLNet_Address* address;
	int port;
	ServerMessageSender* sender;
	SDLNet_DatagramSocket* socket;

	std::vector<ClientInfo*>* connectedClients;

	ClientInfo* connectorInfo;
	int nextClientID;
	bool connectingAClient;

	std::vector<NetworkedObject*>* ownedObjects;
	std::vector<NetworkedObject*>* nonOwnedObjects;

	void ConfirmClientConnection(ClientInfo* client);
	void TryConnectClient(std::string inData, ClientInfo* client);
	bool IsAlreadyConnected(ClientInfo* client);
protected:
public:
	Server(std::string ipAddress, int port);
	void Update(float deltaTime);
	void Broadcast(std::string message);
	void ImportantBroadcast(NetworkMessageTypes type, std::string message);

	int GetConnectedClientCount();
	void SendMessageTo(NetworkMessageTypes type, std::string message, ClientInfo* receiver);
	ImportantMessage* ProcessImportantMessage(NetworkMessage* msg);
	~Server();
};