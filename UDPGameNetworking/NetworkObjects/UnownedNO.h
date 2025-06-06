#pragma once
#include "NetworkObject.h"
#include "../NetworkMessage.h"
#include "NetworkedValue.h"
#include "ObjectDataProcessor.h"
#include <vector>
//A network object owned by another client, it's incoming data is provided by the owning client, 
// and it streams data to the wrapper for rendering
class UnownedNetworkObject : public NetworkObject {
	//TODO basic stream processing
//TODO lerping
//TODO wrapper render instructions
private:
	std::vector<NetworkedValue*>* networkedValues;
	IEngineObject* engineObject;
protected:
public:
	UnownedNetworkObject(IEngineObject* engineObj);
	~UnownedNetworkObject();
	bool StreamDataReceived(NetworkMessage* msg);
};