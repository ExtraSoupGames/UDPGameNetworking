#pragma once
#include "Senders/ServerMessageSender.h"
#include <string>
#include "Wrapper/IWrapper.h"
#include "NetworkObjects/OwnedNO.h"
#include "NetworkObjects/UnownedNO.h"
class Server {
private:
	//Extract to UDPEndpoint
	SDLNet_Address* address;
	int port;
	ServerMessageSender* sender;
	SDLNet_DatagramSocket* socket;

	std::vector<EndpointInfo*>* connectedClients;

	EndpointInfo* connectorInfo;
	int nextClientID;
	bool connectingAClient;

	std::vector<OwnedNetworkObject*>* ownedObjects;
	std::vector<UnownedNetworkObject*>* nonOwnedObjects;

	void ConfirmClientConnection(EndpointInfo* client);
	void TryConnectClient(std::string inData, EndpointInfo* client);
	bool IsAlreadyConnected(EndpointInfo* client);
protected:
public:
	Server(std::string ipAddress, int port);
	void Update(float deltaTime);
	void Broadcast(std::string message);
	void ImportantBroadcast(NetworkMessageTypes type, std::string message);

	int GetConnectedClientCount();
	void SendMessageTo(NetworkMessageTypes type, std::string message, EndpointInfo* receiver);
	ImportantMessage* ProcessImportantMessage(NetworkMessage* msg);
	~Server();
};