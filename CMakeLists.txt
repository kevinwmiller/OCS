cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#ABOUT
project (OCS)
Set (OCS_VERSION_MAJOR "1")
Set (OCS_VERSION_MINOR "0")
Set (OCS_VERSION_PATCH "0")
Set (VERSION "${OCS_VERSION_MAJOR}.${OCS_VERSION_MINOR}.${OCS_VERSION_PATCH}")

#ENVIRONMENT
Set (INCLUDE_PATH 
				${PROJECT_SOURCE_DIR}/include
				${PROJECT_SOURCE_DIR}/include/Components
				${PROJECT_SOURCE_DIR}/include/Messaging
				${PROJECT_SOURCE_DIR}/include/Misc
				${PROJECT_SOURCE_DIR}/include/Objects
				${PROJECT_SOURCE_DIR}/include/States
				${PROJECT_SOURCE_DIR}/include/Systems
				${PROJECT_SOURCE_DIR}/include/Utilities)

Set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)
Set (TEST_DIR ${PROJECT_SOURCE_DIR}/Test_Files)

Set (SRC_FILES ${SRC_DIR}/Components/Component.cc
				${SRC_DIR}/Messaging/Message.cc
				${SRC_DIR}/Messaging/MessageHub.cc
				${SRC_DIR}/Messaging/Transceiver.cc
				${SRC_DIR}/Objects/Object.cc
				${SRC_DIR}/Objects/ObjectManager.cc
				${SRC_DIR}/Objects/ObjectPrototypeLoader.cc
				${SRC_DIR}/States/State.cc
				${SRC_DIR}/States/StateManager.cc
				${SRC_DIR}/Systems/SystemManager.cc
				${SRC_DIR}/Utilities/FileParser.cc
				${SRC_DIR}/Utilities/Serializer.cc
				${SRC_DIR}/Utilities/StringUtilities.cc
				${SRC_DIR}/Utilities/Timer.cc
				${SRC_DIR}/Utilities/TimeStamp.cc)
				

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# C++14: set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

include_directories(${INCLUDE_PATH})

Message ("Binary Tree path: ${PROJECT_BINARY_DIR}")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
add_subdirectory(${TEST_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)

Message(${PROJECT_SOURCE_DIR})
#Message("Sources: ${SRC_FILES}")
#install(TARGETS ${PROJECT_NAME}
#		ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/build/lib
#		LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/build/lib
#		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/build/bin)