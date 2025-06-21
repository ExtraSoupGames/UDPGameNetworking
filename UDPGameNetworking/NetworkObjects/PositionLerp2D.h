#pragma once
#include "NetworkedValue.h"
struct Position {
private:
protected:
public:
	Position(int xVal, int yVal);
	int x;
	int y;
};
struct PositionDataPoint {
private:
protected:
public:
	PositionDataPoint(int xVal, int yVal, int timeVal);
	int x;
	int y;
	int time;
};
class PositionLerp2D : public LerpedValue {
private:
	//for use when streaming out (I.E owned object)
	int x;
	int y;
	//for use when streaming in (I.E. non owned object)
	std::vector<PositionDataPoint*>* dataBuffer;
protected:
public:
	PositionLerp2D(int ID, int initX = 0, int initY = 0);
	Position GetLerpedPosition(int currentTime);
	inline int GetX() { return x; }
	inline int GetY() { return y; }

	void LerpMessageReceived(int xVal, int yVal, int time);
	virtual bool StreamReceived(std::string streamData) override;
	virtual std::string GetStreamData() override;

	void UpdateValue(int xVal, int yVal);

	virtual std::string Debug() override;
};