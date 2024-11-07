cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MyminiSQL)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
file(GLOB_RECURSE SOURCES "src/includes/*.cpp" "src/includes/*.hpp" "src/main.cpp")
add_executable(MyminiSQL ${SOURCES})

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/includes)

# Link libraries
#target_link_libraries(MyminiSQL )
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)