#include "NetworkedValue.h"

LerpedValue::LerpedValue(int ID) : NetworkedValue(ID)
{
}
PositionLerp2D::PositionLerp2D(int ID, int initX, int initY) : LerpedValue(ID)
{
	x = initX;
	y = initY;
}

void PositionLerp2D::LerpMessageReceived(int xVal, int yVal)
{
	//TODO manage incoming lerp queue
	//TODO add timestamps to messages
	//for now lepring will be skipped
	//TODO lerp incoming Values
	x = xVal;
	y = yVal;
}

bool PositionLerp2D::StreamReceived(std::string streamData)
{
	//28 bits for x, 28 for y
	if (streamData.size() != 64) return false; 
	if(NetworkUtilities::IntFromBinaryString(streamData.substr(0,8), 2) != ID) return false;
	int xIn = NetworkUtilities::IntFromBinaryString(streamData.substr(8, 28), 7);
	int yIn = NetworkUtilities::IntFromBinaryString(streamData.substr(36, 28), 7);
	LerpMessageReceived(xIn, yIn);
	return true;
}

std::string PositionLerp2D::GetStreamData()
{
	std::string streamData = "";
	streamData.append(NetworkUtilities::AsBinaryString(ID, 2));
	streamData.append(NetworkUtilities::AsBinaryString(x, 7));
	streamData.append(NetworkUtilities::AsBinaryString(y, 7));
	return streamData;
}
