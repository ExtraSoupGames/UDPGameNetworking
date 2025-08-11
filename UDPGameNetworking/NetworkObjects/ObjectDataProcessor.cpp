#include "ObjectDataProcessor.h"
//TODO convert to use new NetworkedValue class
void ObjectDataProcessor::ExtractDataFromMessage(std::string* objectData, int* msgLength, int* timestamp, int* describedValueCount)
{
	*msgLength = (int)objectData->size();
	*msgLength -= timestampByteCount * 8;
	*timestamp = NetworkUtilities::IntFromBinaryString(objectData->substr(0, timestampByteCount * 8), timestampByteCount * 2);
	*describedValueCount = *msgLength / streamedValueSize;
}

INetworkedValue* ObjectDataProcessor::FindValueByID(const std::vector<INetworkedValue*>* values, int id) {
	for (INetworkedValue* val : *values) {
		if (val->GetID() == id) return val;
	}
	return nullptr;
}
//Object data stream message format:
//first object data ID
//then 64 bit chunks of values
void ObjectDataProcessor::UpdateValues(std::vector<INetworkedValue*>* values, NetworkMessage* msg)
{
	std::string objectData = msg->GetExtraData().substr(objectIDBits);
	int msgLength = 0;
	int msgTimestamp = 0;
	int describedValueCount = 0;
	ExtractDataFromMessage(&objectData, &msgLength, &msgTimestamp, &describedValueCount);

	if (!msgLength || !msgTimestamp || !describedValueCount) return;
	if (describedValueCount < 1) return;

	for (int i = 0; i < describedValueCount; i++) {
		std::string streamData = objectData.substr((timestampByteCount * 8)+ i * streamedValueSize, streamedValueSize);
		int valueID = NetworkUtilities::IntFromBinaryString(streamData.substr(0,8), 2);
		//TODO improve readability of this
		//TODO consider if this should be reworked to not initialize new values, maybe values should be concrete at declaration
		bool valueIsInitialized = false;
		INetworkedValue* val = FindValueByID(values, valueID);
		if (val) {
			val->StreamReceived(streamData.substr(8, 56), msgTimestamp);
		}
		else {
			// Fallback to default PositionLerp2D for now
			auto* newVal = new PositionLerp2D(valueID, 0, 0);
			values->push_back(newVal);
			// Optional: call StreamReceived if you want initial payload processed
			// newVal->StreamReceived(payload, *msgTimestamp);
		}
	}
}

std::string ObjectDataProcessor::ConstructDataStream(std::vector<INetworkedValue*>* values, int time, LibSettings* settings)
{
	std::string msgData = "";
	msgData += NetworkUtilities::AsBinaryString(time, timestampByteCount * 2);
	for (INetworkedValue* val : *values) {
		msgData.append(val->GetStreamData(time, settings));
	}
	return msgData;
}
