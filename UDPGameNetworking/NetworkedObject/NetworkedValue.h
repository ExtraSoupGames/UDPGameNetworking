#pragma once
class NetworkedValue {

};
class LerpedValue : public NetworkedValue {

};
class PositionLerp2D : public LerpedValue {
private:
	int x;
	int y;
protected:
public:
	inline int GetX() { return x; };
	inline int GetY() { return y; };

	void LerpMessageReceived(int xVal, int yVal);
};