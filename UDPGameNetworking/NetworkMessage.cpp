#include "NetworkMessage.h"
#include "NetworkUtilities.h"
NetworkMessage::NetworkMessage()
{
}
NetworkMessage::NetworkMessage(SDLNet_Datagram* datagramToProcess)
{
	Uint8* buffer = (Uint8*)datagramToProcess->buf;
	int bufferLength = datagramToProcess->buflen;
	string outData = NetworkUtilities::UnpackMessage(buffer, bufferLength);
	messageType = NetworkUtilities::UnpackHeader(outData);
	extraData = outData.substr(4);
	fromAddress = SDLNet_RefAddress(datagramToProcess->addr);
	fromPort = datagramToProcess->port;
}

string NetworkMessage::Debug()
{
	return "The type of this message is: " + to_string(messageType) + " And the contents are: " + extraData;
}
NetworkMessageTypes NetworkMessage::GetMessageType() {
	return messageType;
}
string NetworkMessage::GetExtraData()
{
	return extraData;
}
SDLNet_Address* NetworkMessage::GetAddress()
{
	return fromAddress;
}
int NetworkMessage::GetPort()
{
	return fromPort;
}

string NetworkMessage::RemoveStateVerification()
{
	if (extraData.size() < 4) {
		cout << "State verification remove from network message requested of a message smaller than 4 bits" << endl;
		return "";
	}
	string out = extraData.substr(0, 4);
	extraData = extraData.substr(4);
	return out;
}
