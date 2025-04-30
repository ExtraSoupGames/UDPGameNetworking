#pragma once
#include "MessageSender.h"
using namespace std;
class ClientMessageSender : public MessageSender {
private:
	int serverPort;
	SDLNet_Address* server;
public:
	ClientMessageSender(SDLNet_DatagramSocket* socket, SDLNet_Address* serverAddress, int portForServer);
	void SendImportantMessage(NetworkMessageTypes type, string message);
};