#include "ObjectDataProcessor.h"
//Object data stream message format:
//first object data ID
//then 64 bit chunks of values
//TODO somewhere here probably, server/client time synchronisation and offseting
void ObjectDataProcessor::UpdateValues(std::vector<NetworkedValue*>* values, NetworkMessage* msg)
{
	//TODO organise this function to handle errors better, as well as neatly extracting both ID and timestamp from message before iterating through values
	std::string objectData = msg->GetExtraData().substr(objectIDBits);
	int msgLength = (int)objectData.size();
	msgLength -= timestampByteCount * 8;
	int messageTimestamp = NetworkUtilities::IntFromBinaryString(objectData.substr(0, timestampByteCount * 8), timestampByteCount * 2);
	int describedValueCount = msgLength / streamedValueSize;
	if (describedValueCount < 1) return;
	for (int i = 0; i < describedValueCount; i++) {
		std::string streamData = objectData.substr((timestampByteCount * 8)+ i * streamedValueSize, streamedValueSize);
		int valueID = NetworkUtilities::IntFromBinaryString(streamData.substr(0,8), 2);
		//TODO improve readability of this
		//TODO consider if this should be reworked to not initialize new values, maybe values should be concrete at declaration
		bool valueIsInitialized = false;
		for (NetworkedValue* val : *values) {
			if (val->GetID() == valueID) {
				if (val->StreamReceived(streamData.substr(8, 56), messageTimestamp)) {
					//value already exists, doesnt need initializing
					valueIsInitialized = true;
				}
			}
		}
		if (!valueIsInitialized) {
			values->push_back(new PositionLerp2D(valueID, 0, 0)); //TODO initialize with non 0 values
		}
	}
}

std::string ObjectDataProcessor::ConstructDataStream(std::vector<NetworkedValue*>* values, int time)
{
	std::string msgData = "";
	msgData += NetworkUtilities::AsBinaryString(time, timestampByteCount * 2);
	for (NetworkedValue* val : *values) {
		msgData.append(val->GetStreamData());
	}
	return msgData;
}
