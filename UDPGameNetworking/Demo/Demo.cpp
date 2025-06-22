#include "Demo.h"

DemoClient::DemoClient(bool server, int port)
{
	isServer = server;
	wrapper = new DemoWrapper(port);
	clientPlayer = new DemoPlayer(wrapper);
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
	wrapper->Initialize();
	if (isServer) {
		wrapper->StartServer();
	}
	wrapper->StartClient();
	wrapper->RegisterObject(clientPlayer);
	started = true;
	window = SDL_CreateWindow("UDP Game Networking Demo client", 500, 500, 0);
	renderer = SDL_CreateRenderer(window, NULL);
}

void DemoClient::Update()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
	const SDL_FRect rect = clientPlayer->GetRect();
	SDL_RenderRect(renderer, &rect);
	wrapper->DrawOtherPlayers(renderer);
	SDL_RenderPresent(renderer);
	wrapper->Update(1);
}
void DemoClient::Close()
{
}

void DemoClient::HandleInput(SDL_Event& e)
{
	if (SDL_GetKeyboardFocus() == window) {
		clientPlayer->HandleInput(e);
	}
}

Demo::Demo()
{
	client1 = new DemoClient(true, 55511);
	client2 = new DemoClient(false, 55522);
	client2Delay = 500;
}

Demo::~Demo()
{
	delete client1;
	delete client2;
}

void Demo::Start()
{
	client1->Start();
}

void Demo::Update()
{
	//TODO rewrite this function, its a mess
	client1->Update();

	bool client2Connected = false;
	if (client2Delay > 0) {
		client2Delay--;
		if (client2Delay <= 0) {
			client2->Start();
		}
	}
	else {
		client2Connected = true;
	}
	if (client2Connected) {
		client2->Update();
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		client1->HandleInput(e);
		if (client2Connected) {
			client2->HandleInput(e);
		}
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

DemoPlayer::DemoPlayer(DemoWrapper* libWrapper)
{
	x = 5;
	y = 5;
	wrapper = libWrapper;
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
		if (e.key.key == SDLK_D) {
			x += 5;
		}
		if (e.key.key == SDLK_W) {
			y -= 5;
		}
		if (e.key.key == SDLK_S) {
			y += 5;
		}
	}
}

void DemoPlayer::Update(float deltaTime)
{

}

SDL_FRect DemoPlayer::GetRect()
{
	return SDL_FRect{ (float)x, (float)y, 20, 20 };
}

void DemoPlayer::UpdateLibraryValues(std::vector<NetworkedValue*>* values)
{
	((PositionLerp2D*)values->at(0))->UpdateValue(x, y);
}

void DemoPlayer::UpdateEngineValues(std::vector<NetworkedValue*>* values)
{
	Position p = ((PositionLerp2D*)values->at(0))->GetLerpedPosition(wrapper->GetClientTime());
	x = p.x;
	y = p.y;
}
