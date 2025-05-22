#include "UnownedNO.h"

void UnownedNetworkObject::StreamDataReceived(NetworkMessage* msg)
{
	ObjectDataProcessor::UpdateValues(networkedValues, msg);
}
