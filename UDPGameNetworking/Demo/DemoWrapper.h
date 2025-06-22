#pragma once
#include "../Wrapper/IWrapper.h"
#include <vector>
class DemoPlayer; //forward declaration
class DemoWrapper : public IWrapper {
private:
	int plannedPort;

	std::vector<DemoPlayer*>* otherPlayers;
protected:
public:
	DemoWrapper(int port);
	virtual void Update(float deltaTime) override;
	virtual void Initialize() override;
	virtual void RegisterObject(IEngineObject* obj) override;
	virtual void UnregisterObject(int ID) override;
	virtual void RegisterCallback() override;
	virtual void StartClient() override;
	virtual void StartServer() override;
	virtual void ApplySettings() override;
	virtual void InvokeRegisteredCallback(int callbackID) override;
	virtual IEngineObject* NewNetworkedObject(int objectType, bool belongsToClient) override;


	void DrawOtherPlayers(SDL_Renderer* renderer);
	int GetClientTime();
};
