#pragma once

#ifdef _WIN32
#ifdef UDPGAMENETWORKING_EXPORTS
#define UDPGAMENETWORKING_API __declspec(dllexport)
#else
#define UDPGAMENETWORKING_API __declspec(dllimport)
#endif
#else
#define UDPGAMENETWORKING_API
#endif