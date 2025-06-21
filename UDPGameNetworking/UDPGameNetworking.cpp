// UDPGameNetworking.cpp : Defines the entry point for the application.
//

#include "UDPGameNetworking.h"
#include <iostream>
#include "Endpoints/Client.h"
#include "Endpoints/Server.h"

#include "Testing/TestingFunctions.h"
#include "Demo/Demo.h"

//Big TODO
// - Threading for server polling
int main()
{
	if (!SDLNet_Init()) {
		return 0;
	}
	Demo* demo = new Demo();
	demo->Start();
	while (!demo->Done()) {
		demo->Update();
	}
	demo->Close();




	/*Default Test
	std::cout << "Hello CMake." << std::endl;
	IWrapper* wrapper = new TestWrapper();
	Server* s = new Server("127.0.0.1", 66661, wrapper);
	Client* c1 = new Client(55555, wrapper);
	Client* c2 = new Client(54545, wrapper);
	c1->ConnectToServer("127.0.0.1");
	bool done = false;
	c1->SendServerMessage(UserUnImportant, "11111111", "1111");
	//initialize 3 testing objects to test data streaming to server (set up so in theory IDs should match x and y values)
	TestEngineObject* obj = new TestEngineObject(1, 1);
	c1->RegisterObject(obj);
	TestEngineObject* obj2 = new TestEngineObject(2, 2);
	c1->RegisterObject(obj2);
	while (!done) {
		c1->Update(1);
		c2->Update(2);
		s->Update(1);
		if (SDL_GetTicks() > 500) {
			done = true;
		}
	}
	done = false;
	c2->ConnectToServer("127.0.0.1");
	TestEngineObject* obj3 = new TestEngineObject(3, 3);
	c2->RegisterObject(obj3);
	while (!done) {
		c1->Update(1);
		c2->Update(2);
		s->Update(1);
		if (SDL_GetTicks() > 1000) {
			done = true;
		}

	}
	std::cout << "Done" << std::endl;
	*/
	return 0;
}

