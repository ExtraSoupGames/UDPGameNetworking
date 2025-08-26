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
	SDLNet_Quit();
	return 0;
}

