#include "TestingFunctions.h"
void SendDummyMessage(std::string toAddress, int toPort, std::string msg, std::string fromAddress, int fromPort) {
	SDLNet_Address* fromAddr = SDLNet_ResolveHostname(fromAddress.c_str());
	SDLNet_WaitUntilResolved(fromAddr, -1);
	SDLNet_Address* toAddr = SDLNet_ResolveHostname(toAddress.c_str());
	SDLNet_WaitUntilResolved(toAddr, -1);
	SDLNet_DatagramSocket* socket = SDLNet_CreateDatagramSocket(fromAddr, fromPort);
	const void* buf = (void*)msg.data();
	const int buflen = msg.length();
	SDLNet_SendDatagram(socket, toAddr, toPort, buf, buflen);
	SDLNet_UnrefAddress(fromAddr);
	SDLNet_UnrefAddress(toAddr);
	SDLNet_DestroyDatagramSocket(socket);
}