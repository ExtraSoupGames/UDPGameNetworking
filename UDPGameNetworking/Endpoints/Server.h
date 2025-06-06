#pragma once
#include "../Senders/ServerMessageSender.h"
#include <string>
#include <queue>
#include "../NetworkObjects/OwnedNO.h"
#include "../NetworkObjects/UnownedNO.h"
class IWrapper; // // Forward declaration as Wrapper needs server implementation
class Server {
private:
	IWrapper* wrapper;
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
	std::queue<int>* availableIDs;

	int GetNextFreeID();

	void ConfirmClientConnection(EndpointInfo* client);
	void TryConnectClient(std::string inData, EndpointInfo* client);
	bool IsAlreadyConnected(EndpointInfo* client);

	//TODO extract to socket holder - too much in common with ckient
	void PollSocket();
	void ProcessMessage(NetworkMessage* msg);

	void ProcessIncomingIDRequest(NetworkMessage* msg);
	void ProcessUserMessage(NetworkMessage* msg);
	void ProcessObjectMessage(NetworkMessage* msg);
protected:
public:
	Server(std::string ipAddress, int port, IWrapper* libWrapper);
	void Update(float deltaTime);
	void Broadcast(std::string message);
	void ImportantBroadcast(NetworkMessageTypes type, std::string message);

	int GetConnectedClientCount();
	void SendMessageTo(NetworkMessageTypes type, std::string message, EndpointInfo* receiver);
	~Server();
};