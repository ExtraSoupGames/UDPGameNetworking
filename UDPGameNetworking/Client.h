#pragma once
#include "Senders/ClientMessageSender.h"
#include "NetworkObjects/OwnedNo.h"
#include "NetworkObjects/UnownedNO.h"
class IWrapper; // Forward declaration as Wrapper needs client implementation
class Client {
private:
	IWrapper* wrapper;
	//TODO extract to UDPEndpoint
	SDLNet_Address* serverAddress;
	int port;
	ClientMessageSender* sender;
	SDLNet_DatagramSocket* socket;

	std::vector<OwnedNetworkObject*>* ownedObjects; //TODO try changing to a queue as newly created objects are those more frequently searched for?
	std::vector<UnownedNetworkObject*>* nonOwnedObjects;

	void PollSocket();
	void ProcessMessage(NetworkMessage* msg);
	ImportantMessage* ProcessImportantMessage(NetworkMessage* msg);

	void ProcessIncomingIDRequest(NetworkMessage* msg);
	void ProcessUserMessage(NetworkMessage* msg);
	void ProcessObjectMessage(NetworkMessage* msg);
	void ProcessImportantMessageConfirmation(NetworkMessage* msg);
protected:
public:
	Client(int portToUse, IWrapper* libWrapper);
	~Client();	
	
	void ConnectToServer(std::string serverAddress);
	//disconnects from a server (with no warning to the server) and cleares network pointers
	void Disconnect();
	//process incoming packets and trigger relevant callbacks
	void Update(float deltaTime);
	bool IsConnected();
	void SendServerMessage(NetworkMessageTypes type, std::string msg, std::string stateCode);

	//TODO add creating owned objects
};