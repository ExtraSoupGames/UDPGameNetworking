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
	//initialize 3 testing objects to test data streaming to server (set up so in theory IDs should match x and y values)
	TestEngineObject* obj = new TestEngineObject(1, 1);
	c->RegisterObject(obj);
	TestEngineObject* obj2 = new TestEngineObject(2, 2);
	c->RegisterObject(obj2);
	TestEngineObject* obj3 = new TestEngineObject(3, 3);
	c->RegisterObject(obj3);
	while (!done) {
		c->Update(1);
		s->Update(1);
		if (SDL_GetTicks() > 500) {
			done = true;
		}
	}
	std::cout << "Done" << std::endl;
	return 0;
}

