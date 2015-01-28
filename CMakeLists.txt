cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#ABOUT
project (OCS)
Set (OCS_VERSION_MAJOR "1")
Set (OCS_VERSION_MINOR "1")
Set (OCS_VERSION_PATCH "0")
Set (VERSION "${OCS_VERSION_MAJOR}.${OCS_VERSION_MINOR}.${OCS_VERSION_PATCH}")

#ENVIRONMENT

Set(OperatingSystem "Windows")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
   Set(OperatingSystem "Mac OS X")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux specific code
    Set(OperatingSystem "Linux")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

Set (INCLUDE_PATH 
				${PROJECT_SOURCE_DIR}/include
				${PROJECT_SOURCE_DIR}/include/Commands
				${PROJECT_SOURCE_DIR}/include/Components
				${PROJECT_SOURCE_DIR}/include/Messaging
				${PROJECT_SOURCE_DIR}/include/Misc
				${PROJECT_SOURCE_DIR}/include/Objects
				${PROJECT_SOURCE_DIR}/include/States
				${PROJECT_SOURCE_DIR}/include/Systems
				${PROJECT_SOURCE_DIR}/include/Utilities)

Set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)
Set (TEST_DIR ${PROJECT_SOURCE_DIR}/Test_Files)

Set (SRC_FILES  ${SRC_DIR}/Components/Component.cc
				${SRC_DIR}/Components/SentinalType.cc
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

#Create a test script in the build folder
IF (OperatingSystem MATCHES "Windows")
	IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/TestOCS.bat)
			Message("Creating Windows test files")
			file(WRITE build/testOCS.bat "..\\Test_Files\\build\\bin\\OCS_Test.exe")
	ENDIF(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/TestOCS.bat)
ELSEIF (NOT OperatingSystem MATCHES "Windows")
	IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/TestOCS.sh)
		Message("Creating Linux/Mac test files")
		file(WRITE build/testOCS.sh "#!/bin/bash\n../Test_Files/build/bin/OCS_Test.exe")
	ENDIF(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/TestOCS.sh)
ENDIF (OperatingSystem MATCHES "Windows")

#Copy files needed for testing
Message("Copying test files")
IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/prototypes.txt)
	Message("Copying prototype test file")
	file(COPY	Test_Files/prototypes.txt DESTINATION ${PROJECT_SOURCE_DIR}/build)
ENDIF(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/prototypes.txt)

IF(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/Otherfile.txt)
	Message("Copying other prototype test file")
	file(COPY Test_Files/Otherfile.txt DESTINATION ${PROJECT_SOURCE_DIR}/build)
ENDIF(NOT EXISTS ${PROJECT_SOURCE_DIR}/build/Otherfile.txt)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

add_subdirectory(${TEST_DIR})

Message(${PROJECT_SOURCE_DIR})
#Message("Sources: ${SRC_FILES}")
install(TARGETS ${PROJECT_NAME}
		ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/build/bin
		LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/build/bin
		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/build/bin)
		

