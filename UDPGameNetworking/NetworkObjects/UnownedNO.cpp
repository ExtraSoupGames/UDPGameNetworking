#include "UnownedNO.h"

UnownedNetworkObject::UnownedNetworkObject()
{
	networkedValues = new std::vector<NetworkedValue*>();
}

UnownedNetworkObject::~UnownedNetworkObject()
{
	for (NetworkedValue* v : *networkedValues) {
		delete v;
	}
	delete networkedValues;
}

bool UnownedNetworkObject::StreamDataReceived(NetworkMessage* msg)
{
	std::string IDData = msg->GetExtraData().substr(0,8);
	int streamObjectID = NetworkUtilities::IntFromBinaryString(IDData, 2);
	if (streamObjectID != ID) {
		return false;
	}
	ObjectDataProcessor::UpdateValues(networkedValues, msg);
	return true;
}
