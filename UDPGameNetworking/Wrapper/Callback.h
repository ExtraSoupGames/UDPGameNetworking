#pragma once
#include <iostream>
#include "../UDPGameNetworkingAPI.h"
//This class represents a callback that has been registered by the engine user, it will be invoked upon a specified message being received
class UDPGAMENETWORKING_API Callback {
private:
	int ID;
protected:
public:
	Callback(int callbackID);
	virtual void Invoke(std::string optionalExtraData) = 0;
	bool matchesID(int IDToMatch);
};