#pragma once
#include "../Wrapper/LibSettings.h"
class INetworkedValue {
private:
protected:
public:
    virtual ~INetworkedValue() = default;
    virtual const int GetID() = 0;
    virtual bool StreamReceived(std::string streamData, int time) = 0;
    virtual int GetPacketPayloadLength() = 0;
    virtual std::string Debug() = 0;
    virtual std::string GetStreamData(int currentTime, LibSettings* settings) = 0;
};