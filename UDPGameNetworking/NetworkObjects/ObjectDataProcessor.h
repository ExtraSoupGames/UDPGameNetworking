#pragma once
#include <vector>
#include "../CustomStreaming/NetworkedValue.h"
#include "../NetworkMessage.h"
#include "OwnedNO.h"
#include "../CustomStreaming/PositionLerp2D.h"
static const int timestampByteCount = 4;
class ObjectDataProcessor {
private:
	static void ExtractDataFromMessage(std::string* objectData, int* msgLength, int* timestamp, int* describedValueCount);
	static INetworkedValue* FindValueByID(const std::vector<INetworkedValue*>* values, int id);
protected:
public:
	//updates all networked values whose data is included in the network message
	// @param values - the values to be updated
	// @param msg - the network message to process values from
	static void UpdateValues(std::vector<INetworkedValue*>* values, NetworkMessage* msg);
	//constructs a string of 1s and 0s for a data stream detailing current value of all provided networked values
	// @param values - the values whose data will be contained in the stream
	static std::string ConstructDataStream(std::vector<INetworkedValue*>* values, int time, LibSettings* settings);
};