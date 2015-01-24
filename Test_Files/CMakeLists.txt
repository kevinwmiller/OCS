cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#ABOUT
project (OCS_Test)


#ENVIRONMENT
Set (INCLUDE_PATH
				${PROJECT_SOURCE_DIR}
				${PROJECT_SOURCE_DIR}/include
				${PROJECT_SOURCE_DIR}/include/Components
				${PROJECT_SOURCE_DIR}/include/Messaging
				${PROJECT_SOURCE_DIR}/include/Misc
				${PROJECT_SOURCE_DIR}/include/Objects
				${PROJECT_SOURCE_DIR}/include/States
				${PROJECT_SOURCE_DIR}/include/Systems
				${PROJECT_SOURCE_DIR}/include/Utilities)

Set (SRC_DIR ${PROJECT_SOURCE_DIR})

Set (SRC_FILES ${SRC_DIR}/main.cpp
				${SRC_DIR}/MessageHub_Test.cc
				${SRC_DIR}/ObjectManager_Test.cc
				${SRC_DIR}/SystemManager_Test.cc
				${SRC_DIR}/SampleSystems.cc)
				

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${INCLUDE_PATH})

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} OCS)

#Get a copy of the latest OCS librar
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/../build/libOCS.dll"
		${PROJECT_SOURCE_DIR}/build/bin)
#Get test file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${TEST_DIR}/Otherfile.txt"
		${TEST_DIR}/build/bin)

#Get test file
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${TEST_DIR}/prototypes.txt"
		${TEST_DIR}/build/bin)

install(TARGETS ${PROJECT_NAME}
		ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/build/bin
		LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/build/bin
		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/build/bin)
