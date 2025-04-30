#include "ClientMessageSender.h"

ClientMessageSender::ClientMessageSender(SDLNet_DatagramSocket* socket, SDLNet_Address* serverAddress, int portForServer) : MessageSender(socket)
{
	server = serverAddress;
	serverPort = portForServer;
}

void ClientMessageSender::SendImportantMessage(NetworkMessageTypes type, string message)
{
	MessageSender::SendImportantMessageTo(message, type, server, serverPort);
}
