#include "UnownedNO.h"
#include "../Wrapper/IWrapper.h"
UnownedNetworkObject::UnownedNetworkObject(IEngineObject* engineObj, NetworkMessage* msg, IWrapper* wrapper)
{
	ID = NetworkUtilities::GetObjectIDFromMsg(msg);
	engineObject = engineObj;
	networkedValues = new std::vector<INetworkedValue*>();
	ObjectDataProcessor::InitializeValues(networkedValues, msg, wrapper);
}

UnownedNetworkObject::~UnownedNetworkObject()
{
	for (INetworkedValue* v : *networkedValues) {
		delete v;
	}
	delete networkedValues;
	//TODO destroy the engine object
}

bool UnownedNetworkObject::MatchID(int IDToMatch)
{
	return ID == IDToMatch;
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
