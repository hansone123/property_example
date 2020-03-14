cmake_minimum_required(VERSION 3.1)
project (test.run)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

set (CMAKE_CXXFLAGS_DEBUG "$ENV{SXXFLAG} -Wall -g -ggdb")
set (CMAKE_CXXFLAGS_RELEASE "$ENV{CXXFLAG} -Wall")

add_executable(${PROJECT_NAME} main.cpp )
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )
	