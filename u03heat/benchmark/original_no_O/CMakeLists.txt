################################################################################
#  CMake options
################################################################################

PROJECT(heat)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1)

SET(CMAKE_VERBOSE_MAKEFILE ON)


################################################################################
# Project options
################################################################################




################################################################################
#  Project settings
################################################################################

MESSAGE(${PROJECT_NAME} " root directory: " ${PROJECT_SOURCE_DIR})


SET(MY_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/build")

MESSAGE(${PROJECT_NAME} " output directory: " ${MY_OUTPUT_DIR})


SET(LIBRARY_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})
SET(LIBRARY_OUTPUT_PATH      ${MY_OUTPUT_DIR})
SET(RUNTIME_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})
SET(ARCHIVE_OUTPUT_DIRECTORY ${MY_OUTPUT_DIR})
SET(EXECUTABLE_OUTPUT_PATH   ${MY_OUTPUT_DIR})


SET(MY_PROJECT_INCLUDE_DIR include)
SET(MY_PROJECT_SOURCE_DIR src)


################################################################################
# Preprocessor configuration
################################################################################

#ADD_DEFINITIONS( -fno-strict-aliasing )


################################################################################
# Sources
################################################################################

# Only need to list the header files which must be processed by MOC.
# Also templates, if header and implementation are in distinct files.
SET(MY_HEADER_FILES

    ${MY_PROJECT_INCLUDE_DIR}/LaplaceHeat.h

)

SET(MY_SOURCE_FILES

    ${MY_PROJECT_SOURCE_DIR}/LaplaceHeat.cpp
    ${MY_PROJECT_SOURCE_DIR}/heat.cpp
    
)


################################################################################
# Compiler flags
################################################################################

INCLUDE_DIRECTORIES(

    ${MY_PROJECT_INCLUDE_DIR}

)

# CMake sets additional flags for debug and release builds by using the 
# correct build types.
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -g")


################################################################################
# Compiler flags
################################################################################

LINK_DIRECTORIES(

    

)


################################################################################
# Build targets
################################################################################

ADD_EXECUTABLE(${PROJECT_NAME}
    
    ${MY_SOURCE_FILES}

)


################################################################################
# Link targets
################################################################################

TARGET_LINK_LIBRARIES(${PROJECT_NAME}

    

)
