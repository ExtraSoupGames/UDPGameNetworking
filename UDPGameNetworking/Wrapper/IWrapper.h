#pragma once
//This class should be inherited by wrapper to provide service specific
// functionality relating to registering values for consistent broadcasting
class IWrapper {
private:
protected:
public:
	// --- To be called by engine plugin/engine ---
	virtual void Update(float deltaTime) = 0;
	virtual void Initialize() = 0;
	// --- To be called by library ---
	virtual void InvokeRegisteredCallback(int callbackID) = 0;
	virtual void RegisterCallback(int callbackID) = 0;
};