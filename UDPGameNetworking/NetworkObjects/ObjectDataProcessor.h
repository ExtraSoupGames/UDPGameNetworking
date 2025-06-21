#pragma once
#include <vector>
#include "NetworkedValue.h"
#include "../NetworkMessage.h"
#include "OwnedNO.h"
#include "PositionLerp2D.h"
class ObjectDataProcessor {
private:
protected:
public:
	//updates all networked values whose data is included in the network message
	// @param values - the values to be updated
	// @param msg - the network message to process values from
	static void UpdateValues(std::vector<NetworkedValue*>* values, NetworkMessage* msg);
	//constructs a string of 1s and 0s for a data stream detailing current value of all provided networked values
	// @param values - the values whose data will be contained in the stream
	static std::string ConstructDataStream(std::vector<NetworkedValue*>* values);
};