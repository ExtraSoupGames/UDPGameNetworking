#include "ImportantMessage.h"
#include "NetworkUtilities.h"
ImportantMessage::ImportantMessage(NetworkMessage* msg)
{
	messageType = msg->GetMessageType();
	fromAddress = msg->GetAddress();
	fromPort = msg->GetPort();
	//trim off the first 12 bits as they are the important message ID
	extraData = msg->GetExtraData().substr(12);
	messageID = NetworkUtilities::IntFromBinaryString(msg->GetExtraData().substr(0, 12), 3);
	delete msg;
}

ImportantMessage::~ImportantMessage()
{
}

int ImportantMessage::GetMessageID()
{
	return messageID;;
}

string ImportantMessage::Debug()
{
	return "This is an important message with ID: " + to_string(messageID) + " and with type: " + to_string(messageType) + " And the contents are : " + extraData;
}