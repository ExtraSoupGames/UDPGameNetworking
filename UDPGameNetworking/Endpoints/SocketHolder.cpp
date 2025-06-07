#include "SocketHolder.h"
void SocketHolder::PollSocket()
{
	NetworkMessage* nextMessage = nullptr;
	while (NetworkUtilities::GetNextIncoming(socket, nextMessage, GetSender())) {
		ProcessMessage(nextMessage);
		delete nextMessage;
	}
}