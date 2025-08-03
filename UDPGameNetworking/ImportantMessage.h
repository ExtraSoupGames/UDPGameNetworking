#pragma once
#include "NetworkMessage.h"
class ImportantMessage : public NetworkMessage {
private:
protected:
	int messageID;
public:
	ImportantMessage(NetworkMessage* msg);
	virtual ~ImportantMessage() override;
	int GetMessageID();
	virtual std::string Debug() override;
	virtual std::string GetDataToForwardMessage() override;
};