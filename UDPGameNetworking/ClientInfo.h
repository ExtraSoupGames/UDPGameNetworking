#pragma once
#include <SDL3_net/SDL_net.h>
struct ClientInfo {
	SDLNet_Address* address;
	int clientPort; // TODO rename to port
	ClientInfo(SDLNet_Address* pAddress, int pClientPort) {
		address = pAddress;
		clientPort = pClientPort;
	}
	~ClientInfo() {
		delete address;
	}
	bool operator==(const ClientInfo& other) {
		return other.clientPort
			== clientPort
			&& SDLNet_GetAddressString(other.address)
			== SDLNet_GetAddressString(address);
	}
};