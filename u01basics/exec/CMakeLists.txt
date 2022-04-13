cmake_minimum_required(VERSION 3.13)
project(U_01_Basics)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

set (argLib CmdArgs.so)
set(libPath ${CMAKE_CURRENT_SOURCE_DIR}/../libCmdArgs)
include_directories(arglib ${libPath}/include)
link_directories(argLib ${libPath}/build )


include_directories(include)
file(GLOB SOURCES src/main.cpp src/matrixMath.cpp)

add_executable(exec ${SOURCES})
target_link_libraries(exec ${argLib})
target_link_libraries(exec ${ARMADILLO_LIBRARIES})
