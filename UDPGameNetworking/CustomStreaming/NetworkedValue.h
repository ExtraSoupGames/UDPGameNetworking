#pragma once
#include <string>
#include "INetworkedValue.h"
#include "../NetworkUtilities.h"
template<typename T>
class NetworkedValue : public INetworkedValue {
private:
protected:
	int ID;
public:
	NetworkedValue(int valueID) { ID = valueID; };
	virtual const int GetID() override { return ID; };
	//Checks if an incoming stream data segment is relevant to this networked value
	// and, if so, processes it and returns true, otherwise returns false
	// @param streamData - the  64 bits of data containing both the ID and value data
	// should be a 64 character string containing only 1s and 0s
	virtual bool StreamReceived(std::string streamData, int time) = 0;
	//Returns the number of bits included in each value payload
	virtual int GetPacketPayloadLength() = 0;
	virtual T Deserialize(std::string data) = 0;
	virtual std::string GetStreamData(int currentTime, LibSettings* settings) override {
		std::string idBits = NetworkUtilities::AsBinaryString(ID, 2);
		T currentValue = GetCurrentValue(currentTime, settings);
		std::string valueBits = Serialize(currentValue);

		return idBits + valueBits;
	}
	//Returns the relevant data about this value ready for streaming as 64 bits formatted as
	// a string of payload length + 8 characters containing only 1s and 0s
	virtual std::string Serialize(T) = 0;
	virtual std::string Debug() = 0;
	virtual T GetCurrentValue(int currentTime, LibSettings* settings) = 0;
};