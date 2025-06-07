#pragma once
#include <SDL3_net/SDL_net.h>
#include <string>
//manages an endpoint
struct EndpointInfo {
	SDLNet_Address* address;
	int port;
	EndpointInfo(std::string addressString, int pPort) {
		address = SDLNet_ResolveHostname(addressString.c_str());
		SDLNet_WaitUntilResolved(address, -1); //TODO fix timeout
		port = pPort;
	}
	EndpointInfo(SDLNet_Address* addressRef, int pPort) {
		address = SDLNet_RefAddress(addressRef);
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
	bool operator!=(const EndpointInfo& other) {
		return !(*this == other);
	}
};