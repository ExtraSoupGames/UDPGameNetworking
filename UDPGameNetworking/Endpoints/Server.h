#pragma once
#include "../Senders/ServerMessageSender.h"
#include <string>
#include <queue>
#include "../NetworkObjects/OwnedNO.h"
#include "../NetworkObjects/UnownedNO.h"
#include "SocketHolder.h"
class Server : public SocketHolder{
private:
	SDLNet_Address* address;
	int port;

	ServerMessageSender* sender;
	std::vector<EndpointInfo*>* connectedClients;

	EndpointInfo* connectorInfo;
	bool connectingAClient;

	std::vector<OwnedNetworkObject*>* ownedObjects;
	std::vector<UnownedNetworkObject*>* nonOwnedObjects;
	std::queue<int>* availableIDs;

	int GetNextFreeID(); // for objects

	void ConfirmClientConnection(NetworkMessage* msg);
	void TryConnectClient(NetworkMessage* msg);
	bool IsAlreadyConnected(EndpointInfo* client);

	virtual NetworkMessage* ProcessMessage(NetworkMessage* msg) override;

	void ProcessIncomingIDRequest(NetworkMessage* msg);
	void ProcessUserMessage(NetworkMessage* msg);
	void ProcessObjectMessage(NetworkMessage* msg);
protected:
	virtual MessageSender* GetSender() override { return sender; }
public:
	Server(std::string ipAddress, int port, IWrapper* libWrapper);
	void Update(float deltaTime);
	void Broadcast(NetworkMessageTypes type, std::string message);
	void ImportantBroadcast(NetworkMessageTypes type, std::string message);

	int GetConnectedClientCount();
	void SendMessageTo(NetworkMessageTypes type, std::string message, EndpointInfo* receiver);
	~Server();
};