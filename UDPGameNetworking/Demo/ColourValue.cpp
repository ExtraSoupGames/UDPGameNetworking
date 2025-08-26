#include "ColourValue.h"
#include <bitset>
#include <iostream>
ColourValue::ColourValue(int ID, uint32_t startingColour) : NetworkedValue<uint32_t>(ID)
{
	val = startingColour;
}
bool ColourValue::StreamReceived(std::string streamData, int time)
{
	//time variable is unused as colour is not lerped
	val = Deserialize(streamData);
	return true;
}
int ColourValue::GetPacketPayloadLength()
{
	return 32;
}
uint32_t ColourValue::Deserialize(std::string data)
{
	if (data.size() != 32)
	{
		std::cout << "incorrect data size for colour value deseialization, returning 0" << std::endl;
		return 0;
	}

	return static_cast<uint32_t>(std::stoul(data, nullptr, 2));
}

std::string ColourValue::Serialize(uint32_t val)
{
	return (std::bitset<32>(val)).to_string();
}

std::string ColourValue::Debug()
{
	return "Colour value with value of " + std::to_string(val);
}

uint32_t ColourValue::GetCurrentValue(int currentTime, LibSettings* settings)
{
	//currentTime and settings are ignored as colours are not lerped
	return val;
}

void ColourValue::SetColour(uint32_t col)
{
	val = col;
}

std::string ColourValue::GetMetadata()
{
	std::cout << "COLOUR VALUE BEING INITIALIZED HERE!!! <<<<<<" << std::endl;
	return "00000001";
}
