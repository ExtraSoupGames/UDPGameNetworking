#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>
#include "../NetworkUtilities.h"
#include "../ConnectedClient.h"
#include "../ImportantMessage.h"
using namespace std;
struct UnsentMessage {
	int ID;
	ConnectedClient* target;
	NetworkMessageTypes type;
	string message;
	UnsentMessage(string msg, ConnectedClient* msgTarget, int id, NetworkMessageTypes t) {
		message = NetworkUtilities::AsBinaryString(3, id) + msg;
		ID = id;
		target = msgTarget;
		type = t;
	}
	~UnsentMessage() {
		delete target;
	}
};
class MessageSender {
private:
	int updatesUntilResendMessages;
	int resendMessageRate;
protected:
	SDLNet_DatagramSocket* socket;
	vector<UnsentMessage*> messages;
	vector<int> receivedMessages;
	void IncrementNextMessage();
	int nextMessageID;
	MessageSender(SDLNet_DatagramSocket* socket);
	void SendImportantMessageTo(string message, NetworkMessageTypes type, ConnectedClient* client);
	void SendMessageDirect(NetworkMessageTypes type, string message, SDLNet_DatagramSocket* socket, SDLNet_Address* address, int port);
	bool ShouldResendMessages();
public:
	virtual void SendUnsentMessages(bool skipCheck);

	virtual void ConfirmationRecieved(NetworkMessage* confirmationMessage);
	//returns a pointer to the message as an important message and sends confirmation of the message being sent
	//Note that the confirmation message is sent regardless of wether or not it is the first time since the confirmation may not be received
	//Also note that the parameter pointer will be deleted
	//Implementation should look something like this:
	//message = sender->ProcessImportantMessage(message);
	//if (!message) {
	//	return;
	//}
	virtual ImportantMessage* ProcessImportantMessage(NetworkMessage* importantMessage);
	void SendImportantMessageTo(string message, NetworkMessageTypes type, SDLNet_Address* address, int port);
};
