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
	if (x != 0) {
		return;
	}
	if (y != 0) {
		return;
	}
}

bool PositionLerp2D::StreamReceived(std::string streamData)
{
	//28 bits for x, 28 for y
	if (streamData.size() != 56) return false; 
	int xIn = NetworkUtilities::IntFromBinaryString(streamData.substr(0, 28), 7);
	int yIn = NetworkUtilities::IntFromBinaryString(streamData.substr(28, 28), 7);
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

void PositionLerp2D::UpdateValue(int xVal, int yVal)
{
	x = xVal;
	y = yVal;
}

std::string PositionLerp2D::Debug()
{
	return "position lerp with x: " + std::to_string(x) + " and y: " + std::to_string(y);
}

NetworkedValue::~NetworkedValue()
{
}
