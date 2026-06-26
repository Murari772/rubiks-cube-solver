cmake_minimum_required(VERSION 3.10)
project(RubiksCubeSolver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/generator.cpp")

add_executable(RubiksCubeSolver ${SOURCES} src/main.cpp)
add_executable(DatabaseGenerator ${SOURCES} src/generator.cpp)