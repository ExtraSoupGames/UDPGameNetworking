#pragma once
#include "DemoWrapper.h"
#include "../Wrapper/IEngineObject.h"
class DemoPlayer : public IEngineObject {
private:
protected:
public:
	DemoPlayer();
	~DemoPlayer();
	void HandleInput();
	void Update(float deltaTime);

	virtual void UpdateLibraryValues(std::vector<NetworkedValue*>* values) override;
	virtual void UpdateEngineValues(std::vector<NetworkedValue*>* values) override;
};
class DemoEnemy : public IEngineObject {
private:
protected:
public:
};
class DemoClient {
private:
	bool started;
	bool isServer;
	DemoPlayer* clientPlayer;
	DemoWrapper* wrapper;
protected:
public:
	DemoClient(bool isServer);
	~DemoClient();
	void Start();
	void Update();
	void Close();
};
class Demo {
private:
	DemoClient* client1;
	DemoClient* client2;
protected:
public:
	Demo();
	~Demo();
	void Start();
	void Update();
	void Close();
};