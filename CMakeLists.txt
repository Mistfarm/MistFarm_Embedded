set(CMAKE_MAKE_PROGRAM "C:/Espressif/tools/ninja/1.11.1/ninja.exe" CACHE FILEPATH "Ninja" FORCE)
cmake_minimum_required(VERSION 3.10)

# Set some basic project attributes
project (CMakeLists.txt
	VERSION 0.1
	DESCRIPTION "A Hello World Project")

# This project will output an executable file
add_executable(${PROJECT_NAME} CMakeLists.txt.cpp)

# Create a simple configuration header
configure_file(config.h.in config.h)

# Include the configuration header in the build
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
