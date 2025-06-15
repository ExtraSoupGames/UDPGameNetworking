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
	window = nullptr;
	renderer = nullptr;
}

DemoClient::~DemoClient()
{
	delete clientPlayer;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void DemoClient::Start()
{
	wrapper->StartClient();
	if (isServer) {
		wrapper->StartServer();
	}
	wrapper->RegisterObject(clientPlayer);
	started = true;
	window = SDL_CreateWindow("UDP Game Networking Demo client", 500, 500, 0);
	renderer = SDL_CreateRenderer(window, NULL);
}

void DemoClient::Update()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderRect(renderer, clientPlayer->GetRect());
	SDL_RenderPresent(renderer);
	wrapper->Update(1);
}
void DemoClient::Close()
{
}

void DemoClient::HandleInput(SDL_Event& e)
{
	if (SDL_GetMouseFocus() == window) {
		clientPlayer->HandleInput(e);
	}
}

Demo::Demo()
{
	client1 = new DemoClient(true);
	client2 = new DemoClient(false);
}

Demo::~Demo()
{
	delete client1;
	delete client2;
}

void Demo::Start()
{
	client1->Start();
	client2->Start();
}

void Demo::Update()
{
	client1->Update();
	client2->Update();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		client1->HandleInput(e);
		client2->HandleInput(e);
	}
}

void Demo::Close()
{
	client1->Close();
	client2->Close();
}

bool Demo::Done()
{
	return false;
}

DemoPlayer::DemoPlayer()
{
	x = 5;
	y = 5;
}

DemoPlayer::~DemoPlayer()
{
}

void DemoPlayer::HandleInput(SDL_Event& e)
{
	switch (e.type) {
	case SDL_EVENT_KEY_DOWN:
		if (e.key.key == SDLK_A) {
			x -= 5;
		}
	}
}

void DemoPlayer::Update(float deltaTime)
{
}

SDL_FRect* DemoPlayer::GetRect()
{
	SDL_FRect pos = SDL_FRect{ (float)x, (float)y, 20, 20 };
	return &pos;
}

void DemoPlayer::UpdateLibraryValues(std::vector<NetworkedValue*>* values)
{
	((PositionLerp2D*)values->at(0))->UpdateValue(x, 0);
}

void DemoPlayer::UpdateEngineValues(std::vector<NetworkedValue*>* values)
{
	x = ((PositionLerp2D*)values->at(0))->GetX();
	y = ((PositionLerp2D*)values->at(0))->GetY();
}
