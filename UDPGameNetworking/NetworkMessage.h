#pragma once
#include <string>
#include "SDL3_net/SDL_net.h"
#include "Endpoints/EndpointInfo.h"
enum NetworkMessageTypes;
class NetworkMessage {
private:
protected:
	NetworkMessageTypes messageType;
	SDLNet_Address* fromAddress;
	int fromPort;
	std::string extraData;
public:
	NetworkMessage();
	NetworkMessage(SDLNet_Datagram* datagramToProcess);
	virtual std::string Debug();
	NetworkMessageTypes GetMessageType();
	std::string GetExtraData();
	SDLNet_Address* GetAddress();
	int GetPort();
	std::string RemoveStateVerification();
	//IMPORTANT: returned value must be deleted
	EndpointInfo* GetSender();
};