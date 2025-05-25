#pragma once
#include <unordered_map>
#include "Client.h"
#include "Server.h"
//This class should be inherited with a reference to an engine specific object
// for registering
class IEngineObject {};
//This class should be inherited by wrapper to provide service specific
// functionality relating to registering values for consistent broadcasting
class IWrapper {
private:
protected:
	std::unordered_map<int, IEngineObject*> registeredObjects;
	Client* client;
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
};