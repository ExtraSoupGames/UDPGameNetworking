#include "OwnedNO.h"

void OwnedNetworkObject::StreamSend(EndpointInfo* server, SDLNet_DatagramSocket* socket)
{
	NetworkUtilities::SendMessageTo(NetworkedObjectMsg, ObjectDataProcessor::ConstructDataStream(networkedValues), socket, server->address, server->port);
}

void OwnedNetworkObject::SendIDRequest(EndpointInfo* server, SDLNet_DatagramSocket* socket)
{
	NetworkUtilities::SendMessageTo(IDRequest, "", socket, server->address, server->port);
}

OwnedNetworkObject::OwnedNetworkObject(EndpointInfo* server, SDLNet_DatagramSocket* socket)
{
	//Until the server confirms the ID, the ID will be 0 and object will remain uninitialized
	initialized = false;
	ID = 0;
	streamTimer = 0;
	SendIDRequest(server, socket);
}

OwnedNetworkObject::~OwnedNetworkObject()
{
	//Here we will attempt to send a message to the server notifying it of this object's deletion,
	// in the case that this message's sending is unsuccessful, the object will no longer be around
	// to resend it so the server side instance of the object will just be timed out

	//TODO Send a message to the server notifying of deletion
}

bool OwnedNetworkObject::IDRequestReceived(int newID)
{
	if (initialized || ID != 0) {
		return false;
	}
	ID = newID;
	initialized = true;
	return true;
}

void OwnedNetworkObject::Update(float deltaTime, EndpointInfo* server, SDLNet_DatagramSocket* socket)
{
	if (!initialized) {
		//sometimes messages are lost and server doesnt send an ID confirmation, if so,
		// just ask again
		SendIDRequest(server, socket);
		return;
	}
	streamTimer += deltaTime;
	if (streamTimer > objectStreamRate) {
		streamTimer -= objectStreamRate;
		StreamSend(server, socket);
	}
}

