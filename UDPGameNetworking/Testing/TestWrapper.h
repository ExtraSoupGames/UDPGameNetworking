#pragma once
//This is an example of a wrapper, not linked to any engine, just for testing
//This can also function as an example for writing engine specific wrappers
#include "../Wrapper/IWrapper.h"
#include "../Wrapper/Callback.h"
#include "TestEngineObject.h"
class TestWrapper : public IWrapper {
private:
protected:
public:
	// --- To be called by engine plugin/engine ---
	virtual void Update(float deltaTime) override;
	virtual void Initialize() override;
	virtual void RegisterObject(IEngineObject*) override;
	virtual void UnregisterObject(int ID) override;
	virtual void RegisterCallback() override;
	virtual void StartClient() override;
	virtual void StartServer() override;
	virtual void ApplySettings() override;
	// --- To be called by library ---
	virtual void InvokeRegisteredCallback(int callbackID) override;
	virtual IEngineObject* NewNetworkedObject(int objectType) override;
};
class TestCallback : public Callback {
private:
protected:
public:
	TestCallback(int callbackID);
	virtual void Invoke() override;


};