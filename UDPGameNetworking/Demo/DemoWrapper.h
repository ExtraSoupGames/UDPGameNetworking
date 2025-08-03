#pragma once
#include "../Wrapper/IWrapper.h"
#include "../Wrapper/Callback.h"
#include <vector>
class DemoPlayer; //forward declaration
class DemoWrapper : public IWrapper {
private:
	int plannedPort;

	std::vector<DemoPlayer*>* otherPlayers;
	std::vector<Callback*>* registeredCallbacks;
protected:
public:
	DemoWrapper(int port, int lerpDelay, bool lerpEnabled);
	virtual void Update(float deltaTime) override;
	virtual void Initialize() override;
	virtual void RegisterObject(IEngineObject* obj) override;
	virtual void UnregisterObject(int ID) override;
	virtual void RegisterCallback(int ID) override;
	virtual void StartClient() override;
	virtual void StartServer() override;
	virtual void ApplySettings() override;
	virtual void InvokeRegisteredCallback(int callbackID, std::string optionalExtraData) override;
	virtual IEngineObject* NewNetworkedObject(int objectType, bool belongsToClient) override;


	void DrawOtherPlayers(SDL_Renderer* renderer);
	void CallbackTest();
	int GetClientTime();
};
