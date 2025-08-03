#include "Callback.h"

Callback::Callback(int callbackID)
{
	ID = callbackID;
}

bool Callback::matchesID(int IDToMatch)
{
	return IDToMatch == ID;
}
