#pragma once
#include "../Wrapper/IEngineObject.h"
#include "../NetworkObjects/NetworkedValue.h"
#include <string>
class TestEngineObject : public IEngineObject {
private:
	int x;
	int y;
	int ID;
protected:
public:
	//To be used by library for non owned objects
	TestEngineObject(int serverID, int initX, int initY);
	//To be called by wrapper (engine) to when a registration of an object is being made
	TestEngineObject(int x, int y);
	~TestEngineObject();
	virtual void UpdateLibraryValues(std::vector<NetworkedValue*>* values) override;
	virtual void UpdateEngineValues(std::vector<NetworkedValue*>* values) override;
	void SetValue(int setX, int setY);
};