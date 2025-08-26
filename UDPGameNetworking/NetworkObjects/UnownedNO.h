#pragma once
#include "NetworkObject.h"
#include "../NetworkMessage.h"
#include "../CustomStreaming/NetworkedValue.h"
#include "ObjectDataProcessor.h"
#include "../Wrapper/IEngineObject.h"
#include <vector>
//forward declaration
class IWrapper;
//A network object owned by another client, it's incoming data is provided by the owning client, 
// and it streams data to the wrapper for rendering
class UnownedNetworkObject : public NetworkObject {
private:
	std::vector<INetworkedValue*>* networkedValues;
	IEngineObject* engineObject;
protected:
public:
	UnownedNetworkObject(IEngineObject* engineObj, NetworkMessage* msg, IWrapper* wrapper);
	~UnownedNetworkObject();
	bool StreamDataReceived(NetworkMessage* msg, LibSettings* settings);
};