#pragma once
#include "../NetworkUtilities.h"
#include "../Senders/MessageSender.h"
class IWrapper; //forward declaration
class SocketHolder {
private:
protected:
	IWrapper* wrapper;
	SDLNet_DatagramSocket* socket;
	void PollSocket();
	virtual void ProcessMessage(NetworkMessage* msg) = 0;
	virtual MessageSender* GetSender() = 0;
public:

};