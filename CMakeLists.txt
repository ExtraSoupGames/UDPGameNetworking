# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project(UDPGameNetworking)
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


# Detect platform
if(WIN32)
    # Set paths to your prebuilt SDL3, SDL3_image, and SDL3_net
    set(SDL3_INCLUDE_DIR "C:/Libraries/SDL3-3.2.6/include")
    set(SDL3_LIBRARY "C:/Libraries/SDL3-3.2.6/lib/x64/SDL3.lib") 

    set(SDL3_NET_INCLUDE_DIR "C:/Libraries/SDL3_net/include")
    set(SDL3_NET_LIBRARY "C:/Libraries/SDL3_net/lib/x64/SDL3_net.lib")

    elseif(APPLE OR UNIX)
    # Automatically find SDL3, SDL3_image, and SDL3_net on Linux/macOS
    find_package(SDL3 REQUIRED)
    find_package(SDL3_net REQUIRED)
endif()

set(SOURCES
    UDPGameNetworking/UDPGameNetworking.cpp
    UDPGameNetworking/NetworkUtilities.cpp
    UDPGameNetworking/ImportantMessage.cpp
    UDPGameNetworking/NetworkMessage.cpp
    UDPGameNetworking/Testing/TestingFunctions.cpp
    UDPGameNetworking/Endpoints/Server.cpp
    UDPGameNetworking/Endpoints/Client.cpp
    UDPGameNetworking/Endpoints/SocketHolder.cpp
    UDPGameNetworking/NetworkObjects/NetworkObject.cpp
    UDPGameNetworking/NetworkObjects/OwnedNO.cpp
    UDPGameNetworking/NetworkObjects/UnownedNO.cpp
    UDPGameNetworking/NetworkObjects/ObjectDataProcessor.cpp
    UDPGameNetworking/NetworkObjects/NetworkedValue.cpp
    UDPGameNetworking/NetworkObjects/PositionLerp2D.cpp
    UDPGameNetworking/Senders/MessageSender.cpp
    UDPGameNetworking/Senders/ServerMessageSender.cpp
    UDPGameNetworking/Senders/ClientMessageSender.cpp
    UDPGameNetworking/Wrapper/Callback.cpp
    UDPGameNetworking/Demo/Demo.cpp
    UDPGameNetworking/Demo/DemoWrapper.cpp
    UDPGameNetworking/Demo/DemoCallback.cpp
)
add_executable(UDPGameNetworking ${SOURCES})

target_include_directories(UDPGameNetworking PRIVATE 
    ${SDL3_INCLUDE_DIR} 
    ${SDL3_NET_INCLUDE_DIR}
)

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}")
set(LIBS_DIR "${CMAKE_SOURCE_DIR}/UDPGameNetworking/Libraries")

target_link_libraries(UDPGameNetworking 
    ${SDL3_LIBRARY}
    ${SDL3_NET_LIBRARY}
)

add_custom_command(TARGET UDPGameNetworking POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LIBS_DIR}/SDL3.dll
    $<TARGET_FILE_DIR:UDPGameNetworking>)

add_custom_command(TARGET UDPGameNetworking POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LIBS_DIR}/SDL3_net.dll
    $<TARGET_FILE_DIR:UDPGameNetworking>)