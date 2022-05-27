cmake_minimum_required(VERSION 3.13)
project(U_01_Basics)

# include_directories(include)
file(GLOB SOURCES src/main.cpp)
add_executable(main ${SOURCES})

