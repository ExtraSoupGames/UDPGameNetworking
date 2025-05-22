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

bool OwnedNetworkObject::IDRequestReceived(NetworkMessage* msg)
{
	if (initialized || ID != 0) {
		return false;
	}
	ID = NetworkUtilities::IntFromBinaryString(msg->GetExtraData(), objectIDLength);
	initialized = true;
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

