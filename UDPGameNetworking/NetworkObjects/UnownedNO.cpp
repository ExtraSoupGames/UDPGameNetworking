#include "UnownedNO.h"

UnownedNetworkObject::UnownedNetworkObject(IEngineObject* engineObj)
{
	engineObject = engineObj;
	networkedValues = new std::vector<NetworkedValue*>();
}

UnownedNetworkObject::~UnownedNetworkObject()
{
	for (NetworkedValue* v : *networkedValues) {
		delete v;
	}
	delete networkedValues;
	//TODO destroy the engine object
}

bool UnownedNetworkObject::StreamDataReceived(NetworkMessage* msg)
{
	std::string IDData = msg->GetExtraData().substr(0,8);
	int streamObjectID = NetworkUtilities::IntFromBinaryString(IDData, 2);
	if (streamObjectID != ID) {
		return false;
	}
	ObjectDataProcessor::UpdateValues(networkedValues, msg);
	engineObject->UpdateEngineValues(networkedValues);
	return true;
}
