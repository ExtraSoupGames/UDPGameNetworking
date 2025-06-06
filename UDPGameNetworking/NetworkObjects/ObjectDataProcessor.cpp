#include "ObjectDataProcessor.h"
//Object data stream message format:
//first object data ID
void ObjectDataProcessor::UpdateValues(std::vector<NetworkedValue*>* values, NetworkMessage* msg)
{
	int msgLength = (int)msg->GetExtraData().size();
	int describedValueCount = msgLength / streamedValueSize;
	if (describedValueCount < 1) return;
	for (int i = 0; i < describedValueCount; i++) {
		std::string streamData = msg->GetExtraData().substr(i * streamedValueSize, streamedValueSize);
		int valueID = NetworkUtilities::IntFromBinaryString(streamData.substr(0,8), 2);
		for (NetworkedValue* val : *values) {
			if (val->GetID() == valueID) {
				val->StreamReceived(streamData.substr(8, 56));
			}
		}
	}
}

std::string ObjectDataProcessor::ConstructDataStream(std::vector<NetworkedValue*>* values)
{
	//TODO construct a data stream containing all data needed to update all values
	throw new std::exception; // Not yet implemented
}
