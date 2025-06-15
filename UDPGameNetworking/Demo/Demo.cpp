#include "Demo.h"

DemoClient::DemoClient(bool server)
{
	isServer = server;
	//TODO server management
	//TODO create a window and manage it
	//TODO player rendering
	//TODO get networking working
	wrapper = new DemoWrapper();
	clientPlayer = new DemoPlayer();
	started = false;
}

DemoClient::~DemoClient()
{
	delete clientPlayer;
}

void DemoClient::Start()
{
	wrapper->RegisterObject(clientPlayer);
	started = true;
}
