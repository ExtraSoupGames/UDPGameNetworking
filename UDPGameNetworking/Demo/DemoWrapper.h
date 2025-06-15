#pragma once
#include "../Wrapper/IWrapper.h"
class DemoWrapper : public IWrapper {
private:
protected:
public:
	virtual void Update(float deltaTime) override;
	virtual void Initialize() override;
	virtual void RegisterObject(IEngineObject*) override;
	virtual void UnregisterObject(int ID) override;
	virtual void RegisterCallback() override;
	virtual void StartClient() override;
	virtual void StartServer() override;
	virtual void ApplySettings() override;
	virtual void InvokeRegisteredCallback(int callbackID) override;
	virtual IEngineObject* NewNetworkedObject(int objectType) override;
};
