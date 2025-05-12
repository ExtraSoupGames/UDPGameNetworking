// UDPGameNetworking.cpp : Defines the entry point for the application.
//

#include "UDPGameNetworking.h"
#include <iostream>
#include "Client.h"
#include "Server.h"

int main()
{
	if (SDLNet_Init() < 0) {
		return 0;
	}
	std::cout << "Hello CMake." << std::endl;
	Server* s = new Server("127.0.0.1", 66661);
	Client* c = new Client(55555);
	c->ConnectToServer("127.0.0.1");
	bool done = false;
	while (!done) {
		c->SendServerMessage(UserUnImportant, "11111111", "1111");
		c->Update();
		s->Update(1);
		if (SDL_GetTicks() > 10000) {
			done = true;
		}
	}
	return 0;
}
