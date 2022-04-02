cmake_minimum_required(VERSION 3.13)
project(U_01_Basics)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

set ( argLib libCmdArgs.so)
link_directories(argLib ~/Tutorials/u01basics/libCmdArgs/build )
include_directories(argLib ~/Tutorials/u01basics/libCmdArgs/include )

file(GLOB SOURCES "src/*.cpp")
add_executable(exec ${SOURCES})
target_link_libraries(exec ${ARMADILLO_LIBRARIES})
target_link_libraries(exec ${argLib})
