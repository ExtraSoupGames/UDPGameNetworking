#pragma once
#include "NetworkObject.h"
#include "../NetworkMessage.h"
#include "NetworkedValue.h"
#include "ObjectDataProcessor.h"
#include <vector>
//A network object owned by another client, it's incoming data is provided by the owning client, 
// and it streams data to the wrapper for rendering
class UnownedNetworkObject : public NetworkObject {
//TODO lerping
//TODO wrapper render instructions
private:
	std::vector<NetworkedValue*>* networkedValues;
	IEngineObject* engineObject;
protected:
public:
	UnownedNetworkObject(IEngineObject* engineObj, NetworkMessage* msg);
	~UnownedNetworkObject();
	bool StreamDataReceived(NetworkMessage* msg);
};