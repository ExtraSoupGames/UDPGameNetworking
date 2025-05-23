#pragma once

//This class represents a callback that has been registered by the engine user, it will be invoked upon a specified message being received
class Callback {
private:
	int ID;
protected:
public:
	Callback(int callbackID);
	virtual void Invoke() = 0;
};