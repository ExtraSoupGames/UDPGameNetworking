#pragma once
#include <unordered_map>
#include <vector>
#include "../Endpoints/Client.h"
#include "../Endpoints/Server.h"
#include "IEngineObject.h"
//This class should be inherited by wrapper to provide service specific
// functionality relating to registering values for consistent broadcasting
class IWrapper {
private:
protected:
	//objects being registered, currently waiting for an ID from the server
	std::vector<IEngineObject*> registeringOwnedObjects;
	//objects that have been registered, and are currently being streamed
	std::unordered_map<int, IEngineObject*> registeredOwnedObjects;
	//objects that other clients have registered, and are (hopefully) being rendered by this client
	std::unordered_map<int, IEngineObject*> registeredUnownedObjects;
	//Manages networking and server connection
	Client* client;
	//Not necessary for every client, manages multiple client connections
	Server* server;
public:
	// --- To be called by engine plugin/engine ---
	virtual void Update(float deltaTime) = 0;
	virtual void Initialize() = 0;
	virtual void RegisterObject(IEngineObject*) = 0;
	virtual void UnregisterObject(int ID) = 0;
	virtual void RegisterCallback() = 0;
	virtual void StartClient() = 0;
	virtual void StartServer() = 0;
	virtual void ApplySettings() = 0;
	// --- To be called by library ---
	virtual void InvokeRegisteredCallback(int callbackID) = 0;
	//This should define how the engine should respond when another client, or the server,
	// sends information about a new object (typically should instantiate a new object of 
	// the specified type ready for streaming)
	//returns the engine object to continue data streaming with for this new object
	virtual IEngineObject* NewNetworkedObject(int objectType) = 0;
};