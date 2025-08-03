#include "UnownedNO.h"

UnownedNetworkObject::UnownedNetworkObject(IEngineObject* engineObj, NetworkMessage* msg)
{
	ID = NetworkUtilities::GetObjectIDFromMsg(msg);
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

bool UnownedNetworkObject::StreamDataReceived(NetworkMessage* msg, LibSettings* settings)
{
	int streamObjectID = NetworkUtilities::GetObjectIDFromMsg(msg);
	if (streamObjectID != ID) {
		return false;
	}
	ObjectDataProcessor::UpdateValues(networkedValues, msg);
	engineObject->UpdateEngineValues(networkedValues, settings);
	return true;
}
