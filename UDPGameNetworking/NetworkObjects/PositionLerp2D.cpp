#include "PositionLerp2D.h"


Position PositionLerp2D::GetLerpedPosition(int currentTime, LibSettings* settings)
{
    if (!settings->lerpEnabled) {
        while (dataBuffer->size() > 1) {
            dataBuffer->erase(dataBuffer->begin());
        }
        if (dataBuffer->size() > 0) {
            x = dataBuffer->at(0)->x;
            y = dataBuffer->at(0)->y;
        }
        return Position(x, y);
    }
    currentTime -= settings->lerpDelay;
    //only interpolate if there are more than 2 elements in the buffer
    if (dataBuffer->size() >= 2) {
        if (dataBuffer->at(1)->time < currentTime) {
            dataBuffer->erase(dataBuffer->begin());
            //if the time is past the second element of incomingDataBuffer, then we dont need the first element for interpolation anymore so we can remove it
        }
        //check again if the data bufer has more than 2 elements, one may have been deleted since the first check
        if (dataBuffer->size() >= 2) {
            //calculate the duration of the snapshot (the time between the first and second data elements)
            double snapshotDuration = (dataBuffer->at(1)->time - dataBuffer->at(0)->time);
            //calculate time since the start of the snapshot
            double snapshotSinceStart = (currentTime - dataBuffer->at(0)->time);
            //calculate progress through this snapshot
            double snapshotProgress = snapshotSinceStart / snapshotDuration;
            if (snapshotProgress > 1) { //a new packet has taken too long to arrive, no data is available for where the object should currently be so just stay still
                x = clampToScreenSize(dataBuffer->at(1)->x);
                y = clampToScreenSize(dataBuffer->at(1)->y);
            }
            else { //in the range of being able to interpolate
                x = clampToScreenSize(dataBuffer->at(0)->x) + ((clampToScreenSize(dataBuffer->at(1)->x) - clampToScreenSize(dataBuffer->at(0)->x)) * snapshotProgress);
                y = clampToScreenSize(dataBuffer->at(0)->y) + ((clampToScreenSize(dataBuffer->at(1)->y) - clampToScreenSize(dataBuffer->at(0)->y)) * snapshotProgress);
            }
        }
    }
    return Position(x, y);
}
int PositionLerp2D::clampToScreenSize(int clampVal)
{
    int screenSize = 256;
    return ((clampVal % screenSize) + screenSize) % screenSize;
}
PositionLerp2D::PositionLerp2D(int ID, int initX, int initY) : LerpedValue(ID)
{
	x = initX;
	y = initY;
	dataBuffer = new std::vector<PositionDataPoint*>();
}

void PositionLerp2D::LerpMessageReceived(int xVal, int yVal, int time)
{
	dataBuffer->push_back(new PositionDataPoint(xVal, yVal, time));
}

bool PositionLerp2D::StreamReceived(std::string streamData, int time)
{
	//28 bits for x, 28 for y
	if (streamData.size() != 56) return false;
	int xIn = NetworkUtilities::IntFromBinaryString(streamData.substr(0, 28), 7);
	int yIn = NetworkUtilities::IntFromBinaryString(streamData.substr(28, 28), 7);
	LerpMessageReceived(xIn, yIn, time);
	return true;
}

std::string PositionLerp2D::GetStreamData()
{
	std::string streamData = "";
	streamData.append(NetworkUtilities::AsBinaryString(ID, 2));
	streamData.append(NetworkUtilities::AsBinaryString(GetX(), 7));
	streamData.append(NetworkUtilities::AsBinaryString(GetY(), 7));
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

PositionDataPoint::PositionDataPoint(int xVal, int yVal, int timeVal)
{
	x = xVal;
	y = yVal;
	time = timeVal;
}

Position::Position(int xVal, int yVal)
{
	x = xVal;
	y = yVal;
}
