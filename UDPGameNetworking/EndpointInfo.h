#pragma once
#include <SDL3_net/SDL_net.h>
struct EndpointInfo {
	SDLNet_Address* address;
	int port;
	EndpointInfo(SDLNet_Address* pAddress, int pPort) {
		address = SDLNet_RefAddress(pAddress);
		port = pPort;
	}
	~EndpointInfo() {
		SDLNet_UnrefAddress(address);
	}
	bool operator==(const EndpointInfo& other) {
		return other.port
			== port
			&& SDLNet_GetAddressString(other.address)
			== SDLNet_GetAddressString(address);
	}
};