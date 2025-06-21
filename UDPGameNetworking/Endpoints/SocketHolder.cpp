#include "SocketHolder.h"
void SocketHolder::PollSocket()
{
	NetworkMessage* nextMessage = nullptr;
	while (NetworkUtilities::GetNextIncoming(socket, nextMessage, GetSender())) {
		ProcessMessage(nextMessage);
		delete nextMessage;
	}
}

void SocketHolder::StartTimer(int serverTime)
{
	timerOffset = SDL_GetTicks();
	timerOffset -= serverTime;
	//this function should work such that the moment after it is called, GetTime() will return the same as serverTime
	//If this is true then in theory GetTime() will increment in line with the server
	//if this is the server then servertime should just be 0 
}

int SocketHolder::GetTime()
{
	return SDL_GetTicks() - (timerOffset);
}

bool SocketHolder::TimerStarted()
{
	return timerOffset == 0;
}
