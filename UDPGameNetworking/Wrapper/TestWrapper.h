#pragma once
//This is an example of a wrapper, not linked to any engine, just for testing
//This can also function as an example for writing engine specific wrappers
#include "IWrapper.h"
#include "Callback.h"
class TestWrapper : public IWrapper {
private:
protected:
public:
};
class TestCallback : public Callback {
private:
protected:
public:
	TestCallback(int callbackID);
	virtual void Invoke() override;
	virtual void RegisterObject() override;
};