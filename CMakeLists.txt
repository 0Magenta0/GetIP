#---------------==================================================---------------#
#  CMakeLists.txt                                                                #
#  This file is a part of the GetIP distribution released under GNU GPLv3        #
#  Copyright (C) 2021-2023  0Magenta0                                            #
#  https://github.com/0Magenta0/GetIP                                            #
#---------------==================================================---------------#

CMAKE_MINIMUM_REQUIRED (VERSION 3.21 FATAL_ERROR)

IF (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  MESSAGE (FATAL_ERROR "Please create the build directory and remove ${CMAKE_SOURCE_DIR}/CMakeCache.txt")
ENDIF (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)


#--------=========================================--------#
#              General Project Configuration              #
#--------=========================================--------#
PROJECT (
  getip
    VERSION 2.2.2
    DESCRIPTION "Utility to request info about IP from APIs"
    HOMEPAGE_URL "https://github.com/0Magenta0/GetIP"
    LANGUAGES C
)

SET (CMAKE_C_STANDARD 17)
SET (CMAKE_C_EXTENSIONS OFF)
SET (CMAKE_C_STANDARD_REQUIRED ON)
SET (CMAKE_EXPORT_COMPILE_COMMANDS ON)

IF (NOT CMAKE_BUILD_TYPE)
  MESSAGE (STATUS "Build type is not specified! Setting up to RelWithDebInfo.")
  SET (CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF (NOT CMAKE_BUILD_TYPE)

SET (
  CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} \
     -Wall -Wextra -Werror -Wpedantic \
     -Wcast-qual -Wconversion -Wcast-align \
     -Wredundant-decls -Wsign-conversion \
     -Waggregate-return -Wswitch-enum"
)


#--------=========================================--------#
#         Setup Project Sources And Dependencies          #
#--------=========================================--------#
FIND_PACKAGE (CURL REQUIRED)
FIND_PACKAGE (json-c CONFIG)

ADD_EXECUTABLE (${CMAKE_PROJECT_NAME})

TARGET_SOURCES (
  ${CMAKE_PROJECT_NAME} PRIVATE
  src/getip_main.c
  src/getip_args.c
  src/getip_apis.c
  src/getip_usage.c
  src/getip_errors.c
  src/getip_request.c
)

CONFIGURE_FILE (
  ${CMAKE_SOURCE_DIR}/include/getip_usage.h.in
  ${CMAKE_SOURCE_DIR}/include/getip_usage.h
  NEWLINE_STYLE UNIX
)

TARGET_INCLUDE_DIRECTORIES (
  ${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CURL_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES (
  ${CMAKE_PROJECT_NAME} PRIVATE
    CURL::libcurl
    json-c::json-c
)

