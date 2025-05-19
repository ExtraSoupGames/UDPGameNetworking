#pragma once
#include "Senders/ClientMessageSender.h"
#include "NetworkedObject.h"
class Client {
private:
	//TODO extract to UDPEndpoint
	SDLNet_Address* serverAddress;
	int port;
	ClientMessageSender* sender;
	SDLNet_DatagramSocket* socket;

	std::vector<NetworkedObject*>* ownedObjects;
	std::vector<NetworkedObject*>* nonOwnedObjects;
protected:
public:
	Client(int portToUse);
	~Client();	
	
	void ConnectToServer(std::string serverAddress);
	//disconnects from a server (with no warning to the server) and cleares network pointers
	void Disconnect();
	//process incoming packets and trigger relevant callbacks
	void Update(float deltaTime);
	bool IsConnected();
	void SendServerMessage(NetworkMessageTypes type, std::string msg, std::string stateCode);
	ImportantMessage* ProcessImportantMessage(NetworkMessage* msg);

	//TODO add creating owned objects
};