#pragma once
#include <string>
#include "../NetworkUtilities.h"
class NetworkedValue {
private:
protected:
	int ID;
public:
	NetworkedValue(int valueID) { ID = valueID; };
	~NetworkedValue();
	int GetID() { return ID; }; // TODO flesh out and fix and annotate
	//Checks if an incoming stream data segment is relevant to this networked value
	// and, if so, processes it and returns true, otherwise returns false
	// @param streamData - the  64 bits of data containing both the ID and value data
	// should be a 64 character string containing only 1s and 0s
	virtual bool StreamReceived(std::string streamData) = 0;
	//Returns the relevant data about this value ready for streaming as 64 bits formatted as
	// a string of 64 characters containing only 1s and 0s
	virtual std::string GetStreamData() = 0;
	virtual std::string Debug() = 0;
};
class LerpedValue : public NetworkedValue {
private:
protected:
public:
	LerpedValue(int ID);
};