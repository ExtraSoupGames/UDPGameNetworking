#pragma once
#include <vector>
#include "LerpedValue.h"
#include "../NetworkMessage.h"
class ObjectDataProcessor {
private:
protected:
public:
	void UpdateValues(std::vector<LerpedValue*>* values, NetworkMessage* msg);
};