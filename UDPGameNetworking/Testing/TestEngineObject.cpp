#include "TestEngineObject.h"
TestEngineObject::TestEngineObject(int initX, int initY)
{
	x = initX;
	y = initY;
}

TestEngineObject::~TestEngineObject()
{
}

void TestEngineObject::UpdateLibraryValues(std::vector<NetworkedValue*>* values)
{
	int valIndex = 0;
	for (NetworkedValue* val : *values) {
		//update val
		if (valIndex == 0) {
			PositionLerp2D* posVal = (PositionLerp2D*)val; //create function to change value of networked value
			posVal->UpdateValue(x, y);
		}
		valIndex++;
		//update the value with the specified data
		//for now we assume all objects contain only x and y
	}
}
void TestEngineObject::UpdateEngineValues(std::vector<NetworkedValue*>* values) {
	//prints the incoming values
	//TODO expand on this functionality
	std::cout << "incoming values: " << values->size() << std::endl;
}

void TestEngineObject::SetValue(int setX, int setY)
{
	x = setX;
	y = setY;
}
