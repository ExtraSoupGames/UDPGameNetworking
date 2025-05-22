#pragma once
#include "NetworkObject.h"
#include "../EndpointInfo.h"
#include "../NetworkUtilities.h"
#include "SDL3_net/SDL_net.h"
#include "NetworkedValue.h"
static int objectIDLength = 8;
static float objectStreamRate = 100; // one data stream message per X ms
//A network object owned by this client, it's incoming data is provided by the wrapper and it streams data
// to other clients
class OwnedNetworkObject : public NetworkObject {
	//TODO Stream data
private:
	//Owned network objects are created in an unitialized state, they request an ID from the server upon
	// creation and remain in this uninitialized state until the ID is provided by the server
	bool initialized;
	//tracks time since last stream message sent
	float streamTimer;
	//contains all values that will be streamed to the server, should be updated by wrapper in Update method
	std::vector<NetworkedValue*>* networkedValues;

	//Sends a data stream message
	void StreamSend(EndpointInfo* server, SDLNet_DatagramSocket* socket);
	//Sends the server a request for an ID
	void SendIDRequest(EndpointInfo* server, SDLNet_DatagramSocket* socket);
protected:
public:
	//Create the object in an uninitialized state and request an ID from the server
	OwnedNetworkObject(EndpointInfo* server, SDLNet_DatagramSocket* socket);
	//Initialize the object with an ID provided by the server
	//Returns true if object was initialized
	bool IDRequestReceived(NetworkMessage* msg);


	//Updates the object, sending a message if required
	// @param deltaTime - the time in seconds since last update was called on this object
	void Update(float deltaTime, EndpointInfo* server, SDLNet_DatagramSocket* socket); //TODO expand to include value updating
};