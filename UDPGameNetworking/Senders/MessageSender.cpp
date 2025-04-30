#include "MessageSender.h"

void MessageSender::SendMessageDirect(NetworkMessageTypes type, string message, SDLNet_DatagramSocket* socket, SDLNet_Address* address, int port)
{
	NetworkUtilities::SendMessageDirect(type, message, socket, address, port);
}

void MessageSender::IncrementNextMessage()
{
	nextMessageID++;
	if (nextMessageID > 999) {
		nextMessageID = 0;
	}
}

MessageSender::MessageSender(SDLNet_DatagramSocket* pSocket)
{
	socket = pSocket;
	nextMessageID = 1;
	updatesUntilResendMessages = 0;
	resendMessageRate = 100;
}

void MessageSender::SendImportantMessageTo(string message, NetworkMessageTypes type, SDLNet_Address* address, int port)
{
	UnsentMessage* msg = new UnsentMessage(message, new ConnectedClient(address, port), nextMessageID, type);
	messages.push_back(msg);
	IncrementNextMessage();
}

void MessageSender::SendImportantMessageTo(string message, NetworkMessageTypes type, ConnectedClient* client)
{
	UnsentMessage* msg = new UnsentMessage(message, new ConnectedClient(*client), nextMessageID, type);
	messages.push_back(msg);
	IncrementNextMessage();

}

bool MessageSender::ShouldResendMessages()
{
	updatesUntilResendMessages--;
	if (updatesUntilResendMessages < 0) {
		updatesUntilResendMessages = resendMessageRate;
		return true;
	}
	return false;
}

void MessageSender::SendUnsentMessages(bool skipCheck = false)
{
	if (!skipCheck) {
		if (!ShouldResendMessages()) {
			return;
		}
	}
	for (UnsentMessage* message : messages) {

		NetworkUtilities::SendMessageDirect(message->type, message->message, socket, message->target->address, message->target->clientPort);
	}
}

void MessageSender::ConfirmationRecieved(NetworkMessage* confirmationMessage)
{
	int messageID = NetworkUtilities::IntFromBinaryString(confirmationMessage->GetExtraData().substr(0, 12), 3);
	messages.erase(remove_if(messages.begin(), messages.end(), [messageID](UnsentMessage* m) {if (m->ID == messageID) { delete m; return true; } return false; }), messages.end());
}
ImportantMessage* MessageSender::ProcessImportantMessage(NetworkMessage* importantMessage)
{
	//send the confirmation regardless of wether or not the message is new
	SDLNet_Address* address = importantMessage->GetAddress();
	int port = importantMessage->GetPort();
	NetworkUtilities::SendMessageTo(ImportantMessageConfirmation, importantMessage->GetExtraData(), socket, address, port);
	//check if the message is new, and if so, register it and return true
	int messageID = NetworkUtilities::IntFromBinaryString(importantMessage->GetExtraData().substr(0, 12), 3);
	if (find(receivedMessages.begin(), receivedMessages.end(), messageID) == receivedMessages.end()) {
		receivedMessages.push_back(messageID);
		return new ImportantMessage(importantMessage);
	}
	return nullptr;
}
