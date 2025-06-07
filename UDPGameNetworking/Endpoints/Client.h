#pragma once
#include "../Senders/ClientMessageSender.h"
#include "../NetworkObjects/OwnedNo.h"
#include "../NetworkObjects/UnownedNO.h"
#include "../Wrapper/IEngineObject.h"
class IWrapper; // Forward declaration as Wrapper needs client implementation
class Client {
private:
	IWrapper* wrapper;
	EndpointInfo* serverInfo;
	int port;
	bool isConnected;

	ClientMessageSender* sender;
	SDLNet_DatagramSocket* socket;

	std::vector<OwnedNetworkObject*>* ownedObjects; //TODO try changing to a queue as newly created objects are those more frequently searched for?
	std::vector<UnownedNetworkObject*>* nonOwnedObjects;

	//TODO extract to socket holder - too much in common with server
	void PollSocket();
	void ProcessMessage(NetworkMessage* msg);

	void ProcessIncomingIDRequest(NetworkMessage* msg);
	void ProcessUserMessage(NetworkMessage* msg);
	void ProcessObjectMessage(NetworkMessage* msg);

	void UpdateObjects(float deltaTime);

	bool AmIThisObjectsOwner(int objectID);

	void SendConnectRequest();
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

	void RegisterObject(IEngineObject* obj);
};