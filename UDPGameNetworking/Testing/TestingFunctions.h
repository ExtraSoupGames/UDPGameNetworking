#pragma once
#include <string>
#include "SDL3_net/SDL_net.h"

void SendDummyMessage(std::string toAddress, int toPort, std::string msg, std::string fromAddress, int fromPort);
