#pragma once
//This class should be inherited by wrapper to provide service specific
// functionality relating to registering values for consistent broadcasting
class IWrapper {
private:
protected:
public:
	//As methods are registered by the wrapper this method should return the
	// desired string of 1s and 0s to be transmitted based on the value that
	// has been registered under valueID
	virtual std::string GetValueToTransmit(int valueID) = 0;
};