#pragma once
#include "../Wrapper/Callback.h"
class DemoCallback : public Callback {
private:
protected:
public:
	DemoCallback(int ID);
	virtual void Invoke() override;
};