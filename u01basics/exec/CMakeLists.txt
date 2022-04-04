cmake_minimum_required(VERSION 3.13)
project(U_01_Basics)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})

set ( argLib libCmdArgs.so)
include_directories(argLib ../libCmdArgs/include )
link_directories(argLib ../libCmdArgs/build )
# This is bad right? Would normall install it into /usr/lib/ ?


include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(exec ${SOURCES})
target_link_libraries(exec ${argLib})
target_link_libraries(exec ${ARMADILLO_LIBRARIES})
