#pragma once
#include "../Wrapper/IEngineObject.h"
#include "../NetworkObjects/NetworkedValue.h"
#include <string>
class TestEngineObject : public IEngineObject {
private:
	int x;
	int y;
protected:
public:
	TestEngineObject(int initX, int initY);
	~TestEngineObject();
	virtual void UpdateLibraryValues(std::vector<NetworkedValue*>* values) override;
	virtual void UpdateEngineValues(std::vector<NetworkedValue*>* values) override;
	void SetValue(int setX, int setY);
};