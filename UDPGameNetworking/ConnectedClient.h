#pragma once
#include <SDL3_net/SDL_net.h>
struct ConnectedClient {
	SDLNet_Address* address;
	int clientPort;
	ConnectedClient(SDLNet_Address* pAddress, int pClientPort) {
		address = pAddress;
		clientPort = pClientPort;
	}
};