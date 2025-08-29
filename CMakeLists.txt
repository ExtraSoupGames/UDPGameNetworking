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

set(LIBRARY_SOURCES
    UDPGameNetworking/NetworkUtilities.cpp
    UDPGameNetworking/ImportantMessage.cpp
    UDPGameNetworking/NetworkMessage.cpp
    UDPGameNetworking/Endpoints/Server.cpp
    UDPGameNetworking/Endpoints/Client.cpp
    UDPGameNetworking/Endpoints/SocketHolder.cpp
    UDPGameNetworking/NetworkObjects/NetworkObject.cpp
    UDPGameNetworking/NetworkObjects/OwnedNO.cpp
    UDPGameNetworking/NetworkObjects/UnownedNO.cpp
    UDPGameNetworking/NetworkObjects/ObjectDataProcessor.cpp
    UDPGameNetworking/CustomStreaming/PositionLerp2D.cpp
    UDPGameNetworking/CustomStreaming/NetworkedValue.cpp
    UDPGameNetworking/Senders/MessageSender.cpp
    UDPGameNetworking/Senders/ServerMessageSender.cpp
    UDPGameNetworking/Senders/ClientMessageSender.cpp
    UDPGameNetworking/Wrapper/Callback.cpp
)
set(DEMO_SOURCES
    UDPGameNetworking/UDPGameNetworking.cpp
    UDPGameNetworking/Testing/TestingFunctions.cpp
    UDPGameNetworking/Demo/Demo.cpp
    UDPGameNetworking/Demo/DemoWrapper.cpp
    UDPGameNetworking/Demo/DemoCallback.cpp
    UDPGameNetworking/Demo/DemoColourSquare.cpp
    UDPGameNetworking/Demo/ColourValue.cpp
)
# 1. Build a shared library from the core source files
add_library(UDPGameNetworking SHARED ${LIBRARY_SOURCES})

target_compile_definitions(UDPGameNetworking PRIVATE UDPGAMENETWORKING_EXPORTS)

# 2. Expose the public header files and dependencies
target_include_directories(UDPGameNetworking PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/UDPGameNetworking>
    $<BUILD_INTERFACE:${SDL3_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${SDL3_NET_INCLUDE_DIR}>
)

# 3. Link the library against SDL3 and SDL3_net
target_link_libraries(UDPGameNetworking PUBLIC
    ${SDL3_LIBRARY}
    ${SDL3_NET_LIBRARY}
)

# 4. Build a separate executable for the demo
add_executable(DemoApp ${DEMO_SOURCES})
add_dependencies(DemoApp UDPGameNetworking)


# 5. Link the demo executable to your new library
target_link_libraries(DemoApp PRIVATE UDPGameNetworking)

# 6. Add installation rules to package the library for others
install(TARGETS UDPGameNetworking
    EXPORT UDPGameNetworkingTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Get the path to the SDL DLLs
set(LIBS_DIR "${CMAKE_SOURCE_DIR}/UDPGameNetworking/Libraries")

# Use a custom command to copy the SDL3 DLL to the build output directory
add_custom_command(TARGET DemoApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${LIBS_DIR}/SDL3.dll"
    $<TARGET_FILE_DIR:DemoApp>
)

# Use a custom command to copy the SDL3_net DLL to the build output directory
add_custom_command(TARGET DemoApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${LIBS_DIR}/SDL3_net.dll"
    $<TARGET_FILE_DIR:DemoApp>
)

# Install the public header files
install(DIRECTORY UDPGameNetworking/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Install the necessary SDL3 DLLs with the library
install(FILES
    "${LIBS_DIR}/SDL3.dll"
    "${LIBS_DIR}/SDL3_net.dll"
    DESTINATION bin
    OPTIONAL
)

# This creates a configuration file for use with find_package()
install(EXPORT UDPGameNetworkingTargets
    FILE UDPGameNetworkingConfig.cmake
    NAMESPACE UDPGameNetworking::
    DESTINATION lib/cmake/UDPGameNetworking
)