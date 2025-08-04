#pragma once
#include "../NetworkObjects/NetworkedValue.h"
#include <iostream>
template<typename T>
class CustomStreamedValue : NetworkedValue {
private:
protected:
public:
	virtual int GetPacketPayloadLength() = 0;
	virtual T Deserialize(std::string) = 0;
	virtual std::string Serialize(T) = 0
};