#pragma once
#include <string>
#include "SDL3_net/SDL_net.h"
enum NetworkMessageTypes;
using namespace std;
class NetworkMessage {
private:
protected:
	NetworkMessageTypes messageType;
	SDLNet_Address* fromAddress;
	int fromPort;
	string extraData;
public:
	NetworkMessage();
	NetworkMessage(SDLNet_Datagram* datagramToProcess);
	virtual string Debug();
	NetworkMessageTypes GetMessageType();
	string GetExtraData();
	SDLNet_Address* GetAddress();
	int GetPort();
	string RemoveStateVerification();
};