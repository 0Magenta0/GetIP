#---------------==================================================---------------#
#  CMakeLists.txt                                                                #
#  This file is a part of the GetIP distribution released under GNU GPLv3        #
#  Copyright (C) 2021-2023  0Magenta0                                            #
#  https://github.com/0Magenta0/GetIP                                            #
#---------------==================================================---------------#

CMAKE_MINIMUM_REQUIRED (VERSION 3.21 FATAL_ERROR)

IF (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  MESSAGE (FATAL_ERROR "Please create the build directory and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
ENDIF ()


#--------=========================================--------#
#              General Project Configuration              #
#--------=========================================--------#
PROJECT (
  geitp
    VERSION 2.0
    DESCRIPTION "Utility to get info about IP"
    LANGUAGES C
)

SET (CMAKE_C_STANDARD 17)
SET (CMAKE_C_STANDARD_REQUIRED ON)
SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

IF (NOT CMAKE_BUILD_TYPE)
  MESSAGE (STATUS "Build type is not specified! Setting up to Release.")
  SET (CMAKE_BUILD_TYPE Release)
ENDIF (NOT CMAKE_BUILD_TYPE)

ADD_EXECUTABLE (${CMAKE_PROJECT_NAME} src/main.c)

