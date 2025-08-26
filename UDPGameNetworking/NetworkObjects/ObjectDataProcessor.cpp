#include "ObjectDataProcessor.h"
#include "../Wrapper/IWrapper.h"
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
	//this is onyl called by an object who already knows it is the specified object so we can ignore first 8 bits
	std::string objectData = msg->GetExtraData().substr(objectIDBits);
	int timestamp = NetworkUtilities::IntFromBinaryString(objectData.substr(0, timestampByteCount * 8), timestampByteCount * 2);
	objectData = objectData.substr(timestampByteCount * 8);

	//Process incoming data stream one value at a time
	bool done = false;
	while (!done) {
		int valueID = NetworkUtilities::IntFromBinaryString(objectData.substr(0, 8), 2);
		INetworkedValue* val = FindValueByID(values, valueID);


		if (val) {
			val->StreamReceived(objectData.substr(8, val->GetPacketPayloadLength()), timestamp);
			objectData = objectData.substr(8 + val->GetPacketPayloadLength());
		}
		else {
			std::cout << "Unknown value ID received: " << valueID << " Skipping update." << std::endl;
			done = true;
		}


		//if this was the last value then loop will terminate
		if (objectData.size() < 9) {
			done = true;
		}
	}


}
void ObjectDataProcessor::InitializeValues(std::vector<INetworkedValue*>* values, NetworkMessage* msg, IWrapper* wrapper)
{
	std::string objectData = msg->GetExtraData().substr(objectIDBits + 8);
	int valueCount = NetworkUtilities::IntFromBinaryString(objectData.substr(0, 8), 2);
	objectData = objectData.substr(8);

	for (int i = 0; i < valueCount; i++) {
		int valueID = NetworkUtilities::IntFromBinaryString(objectData.substr(0, 8), 2);
		int valueType = NetworkUtilities::IntFromBinaryString(objectData.substr(8, 8), 2);
		std::cout << "PROCESSING..." << valueType << std::endl;
		objectData = objectData.substr(16);	
		INetworkedValue* newVal = wrapper->NewNetworkedValue(valueID, valueType);
		if (newVal) {
			values->push_back(newVal);
		}
		else {
			std::cout << "Value type could not be created by wrapper, type: " << valueType << std::endl;
		}
	}
	std::cout << "Unowned Network Object initialized, value count: " << valueCount << std::endl;
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
