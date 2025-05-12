#pragma once
#include "NetworkMessage.h"
class ImportantMessage : public NetworkMessage {
private:
protected:
	int messageID;
public:
	//Note that this deletes the parameter message
	ImportantMessage(NetworkMessage* msg);
	~ImportantMessage();
	int GetMessageID();
	virtual std::string Debug() override;
};