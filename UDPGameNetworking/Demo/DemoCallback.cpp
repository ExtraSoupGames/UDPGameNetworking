#include "DemoCallback.h"
#include <iostream>
DemoCallback::DemoCallback(int ID) : Callback(ID)
{
}
void DemoCallback::Invoke(std::string optionalExtraData) {
	std::cout << "Demo Callback Invoked!" << std::endl;
	std::cout << "optional data included: " << optionalExtraData << std::endl;
}