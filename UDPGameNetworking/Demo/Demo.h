#pragma once
#include "DemoWrapper.h"
#include "../Wrapper/IEngineObject.h"
class DemoPlayer : public IEngineObject {
private:
protected:
	int x;
	int y;
public:
	DemoPlayer();
	~DemoPlayer();
	void HandleInput(SDL_Event& e);
	void Update(float deltaTime);

	SDL_FRect GetRect();

	virtual void UpdateLibraryValues(std::vector<NetworkedValue*>* values) override;
	virtual void UpdateEngineValues(std::vector<NetworkedValue*>* values) override;
};
class DemoClient {
private:
	bool started;
	bool isServer;
	DemoPlayer* clientPlayer;
	DemoWrapper* wrapper;
	SDL_Window* window;
	SDL_Renderer* renderer;
protected:
public:
	DemoClient(bool isServer, int port);
	~DemoClient();
	void Start();
	void Update();
	void Close();

	void HandleInput(SDL_Event& e);
};
class Demo {
private:
	int client2Delay; //Delay client 2s connection to prevent simultaneous connections
	//TODO allow simultaneous connecting
	DemoClient* client1;
	DemoClient* client2;
protected:
public:
	Demo();
	~Demo();
	void Start();
	void Update();
	void Close();

	bool Done();
};