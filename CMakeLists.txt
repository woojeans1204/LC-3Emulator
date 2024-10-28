cmake_minimum_required(VERSION 3.10)
project(LC3AssemblerSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)
include_directories(include/Simulator)

# Source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)
# main은 빼고
list(REMOVE_ITEM SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Executable & Library
add_library(LC3AssemblerSimulator STATIC ${SRC_FILES})
#add_executable(LC3SimulatorMain src/main.cpp)

#target_link_libraries(LC3SimulatorMain PRIVATE LC3AssemblerSimulator)
# Add testing
add_subdirectory(tests)
