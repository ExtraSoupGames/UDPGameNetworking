#pragma once
#include "../CustomStreaming/NetworkedValue.h"

class ColourValue : public NetworkedValue<uint32_t> {
private:
	uint32_t val;
protected:
public:
	virtual bool StreamReceived(std::string streamData, int time) override;

	virtual int GetPacketPayloadLength() override;

	virtual uint32_t Deserialize(std::string data) override;

	virtual std::string Serialize(uint32_t val) override;

	virtual std::string Debug() override;

	virtual uint32_t GetCurrentValue(int currentTime, LibSettings* settings) override;

	void SetColour(uint32_t col);
};