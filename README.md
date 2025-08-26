# UDPGameNetworking Library
UDPGameNetworking is a C++ library for game development networking, built on UDP. It is designed with Engine Agnosticity in mind, providing core functionality
that can be wrapped or extended for use in game engines. Currently working for 2 clients running on one system, untested on LAN, NAT traversal not implemented or planned.

### Features
- Generic data streaming over UDP
- Networked value lerping
- Important message confirmation
- Engine agnostic design
- Simple demo to showcase engine functionality

### Demo
- Shows 2 Clients each controlling a cube whose location is streamed with the library
- WASD movement controls
- Hover with mouse over which client window to control that client
- Also shows 2 coloured squares
- Each client's own square is shown on the left of that client's screen
- Press RGB on keyboard to change own squares colour
- Updates on other client's screen

### Installation
- Download the repo and build with CMake
- Run UDPGameNetworking.cpp to see the demo

### Status
This project is still in development, the Demo works, and the project can be used for engineless development, but builds are not yet ready for a full library, and changes are going to be made.

### Planned
- Cleanup
- Library building
- Wrapper development
