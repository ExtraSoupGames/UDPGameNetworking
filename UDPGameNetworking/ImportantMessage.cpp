#include "ImportantMessage.h"
#include "NetworkUtilities.h"
ImportantMessage::ImportantMessage(NetworkMessage* msg)
{
	messageType = msg->GetMessageType();
	fromAddress = msg->GetAddress();
	fromPort = msg->GetPort();
	if (msg->GetDataToForwardMessage().length() < 12) {
		std::string error = "Unexpected important message length, terminating. " + msg->Debug();
		throw std::runtime_error(error);
	}
	//trim off the first 12 bits as they are the important message ID
	extraData = msg->GetDataToForwardMessage().substr(12);
	messageID = NetworkUtilities::IntFromBinaryString(msg->GetDataToForwardMessage().substr(0, 12), 3);
}

ImportantMessage::~ImportantMessage()
{
}

int ImportantMessage::GetMessageID()
{
	return messageID;;
}

std::string ImportantMessage::Debug()
{
	return "This is an important message with ID: " + std::to_string(messageID) + " and with type: " + std::to_string(messageType) + " And the contents are : " + extraData;
}

std::string ImportantMessage::GetDataToForwardMessage()
{
	//Here we add the messageID back so it wont be lost when the message is forwarded
	return NetworkUtilities::AsBinaryString(messageID, 3) + GetExtraData();
}
