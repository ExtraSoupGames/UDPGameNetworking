#pragma once
#include <SDL3_net/SDL_net.h>
struct ClientInfo {
	SDLNet_Address* address;
	int port;
	ClientInfo(SDLNet_Address* pAddress, int pPort) {
		address = pAddress;
		port = pPort;
	}
	~ClientInfo() {
		delete address;
	}
	bool operator==(const ClientInfo& other) {
		return other.port
			== port
			&& SDLNet_GetAddressString(other.address)
			== SDLNet_GetAddressString(address);
	}
};