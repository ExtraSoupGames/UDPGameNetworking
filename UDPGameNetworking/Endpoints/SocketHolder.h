#pragma once
#include "../NetworkUtilities.h"
#include "../Senders/MessageSender.h"
class IWrapper; //forward declaration
class SocketHolder {
private:
	int timerOffset = 0;
protected:
	IWrapper* wrapper;
	SDLNet_DatagramSocket* socket;
	void PollSocket();
	virtual void ProcessMessage(NetworkMessage* msg) = 0;
	virtual MessageSender* GetSender() = 0;
	void StartTimer(int serverTime);
	bool TimerStarted();
public:
	int GetTime();
};