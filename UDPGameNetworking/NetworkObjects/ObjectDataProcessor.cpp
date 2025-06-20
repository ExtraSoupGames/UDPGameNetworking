#include "ObjectDataProcessor.h"
//Object data stream message format:
//first object data ID
void ObjectDataProcessor::UpdateValues(std::vector<NetworkedValue*>* values, NetworkMessage* msg)
{
	std::string objectData = msg->GetExtraData().substr(objectIDBits);
	int msgLength = (int)objectData.size();
	int describedValueCount = msgLength / streamedValueSize;
	if (describedValueCount < 1) return;
	for (int i = 0; i < describedValueCount; i++) {
		std::string streamData = objectData.substr(i * streamedValueSize, streamedValueSize);
		int valueID = NetworkUtilities::IntFromBinaryString(streamData.substr(0,8), 2);
		//TODO improve readability of this
		//TODO consider if this should be reworked to not initialize new values, maybe values should be concrete at declaration
		bool valueIsInitialized = false;
		for (NetworkedValue* val : *values) {
			if (val->GetID() == valueID) {
				if (val->StreamReceived(streamData.substr(8, 56))) {
					//value already exists, doesnt need initializing
					valueIsInitialized = true;
				}
			}
		}
		if (!valueIsInitialized) {
			values->push_back(new PositionLerp2D(valueID, 0, 0));
		}
	}
}

std::string ObjectDataProcessor::ConstructDataStream(std::vector<NetworkedValue*>* values)
{
	std::string msgData = "";
	//TODO construct a data stream containing all data needed to update all values
	for (NetworkedValue* val : *values) {
		msgData.append(val->GetStreamData());
	}
	return msgData;
}
