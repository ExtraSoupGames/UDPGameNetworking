// UDPGameNetworking.cpp : Defines the entry point for the application.
//

#include "UDPGameNetworking.h"
#include <iostream>
#include "Endpoints/Client.h"
#include "Endpoints/Server.h"
#include "Testing/TestWrapper.h"

#include "Testing/TestingFunctions.h"

int main()
{
	if (!SDLNet_Init()) {
		return 0;
	}
	std::cout << "Hello CMake." << std::endl;
	IWrapper* wrapper = new TestWrapper();
	Server* s = new Server("127.0.0.1", 66661, wrapper);
	Client* c = new Client(55555, wrapper);
	c->ConnectToServer("127.0.0.1");
	bool done = false;
	c->SendServerMessage(UserUnImportant, "11111111", "1111");
	while (!done) {
		c->Update(1);
		s->Update(1);
		if (SDL_GetTicks() > 10000) {
			done = true;
		}
	}
	std::cout << "Done" << std::endl;
	return 0;
}

