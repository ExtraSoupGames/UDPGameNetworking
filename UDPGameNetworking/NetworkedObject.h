#pragma once


//This class defines an object whose data will be streamed over the server
//This could either be a server owned object (an object whose definite data
// is stored on the server), or a client owned object (an object whose definite data is stored on a client)
class NetworkedObject {
private:
	int ID;
protected:
public:
	/*
	owned objects need to:
		stream their data
	non owned objects need to:
		process incoming stream data
		tell wrapper how to render them
	*/
	//so little to no functionality is shared
	//TODO seperate into owned and non owned
	//For owned:
	//TODO Stream data
	//For non owned:
	//TODO basic stream processing
	//TODO lerping
	//TODO wrapper render instructions
};