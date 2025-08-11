#pragma once
#include <vector>
#include "../CustomStreaming/NetworkedValue.h"
#include "../Wrapper/LibSettings.h"
//This class should be inherited with a reference to an engine specific object
// for registering
class IEngineObject {
private:
	//This holds a reference to an engine object and provides interface for the library to send and receive data for it
	//this involves owned and non owned network object requirements

protected:
public:
	//constructor (library initialized (non owned))
	//constructor (engine initialized (owned))
	//set value (owned)
	//get value (non owned)

	//should this be 2 classes
	//make a test one
	virtual void UpdateLibraryValues(std::vector<INetworkedValue*>* values) = 0;
	virtual void UpdateEngineValues(std::vector<INetworkedValue*>* values, LibSettings* settings) = 0;
};