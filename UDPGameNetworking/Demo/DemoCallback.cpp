#include "DemoCallback.h"
#include <iostream>
DemoCallback::DemoCallback(int ID) : Callback(ID)
{
}
void DemoCallback::Invoke() {
	std::cout << "Demo Callback Invoked!" << std::endl;
}