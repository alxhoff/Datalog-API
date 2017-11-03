#
# @file        datalog_api.c
# @author      Alexander Hoffman
# @email       alxhoff@gmail.com
# @website     http://alexhoffman.info
# @copyright   GNU GPL v3
# @brief       API functions to use the LUA/C datalog implementation found
#              found at http://datalog.sourceforge.net
#  
#@verbatim
#   ----------------------------------------------------------------------
#    Copyright (C) Alexander Hoffman, 2017
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#   ----------------------------------------------------------------------
#@endverbatim
#

project(DATALOG_API)
cmake_minimum_required (VERSION 3.4 FATAL_ERROR)

#debug
set(CMAKE_BUILD_TYPE Debug)

#exec output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

#Versioning
set(DATALOGAPI_VERSION_MAJOR 1)
set(DATALOGAPI_VERSION_MINOR 0)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(warnings "-Wall -Wextra -Werror")
endif()

#cmake config
configure_file(
    "${PROJECT_SOURCE_DIR}/config.h"
    "${PROJECT_BINARY_DIR}/config.h"
    )

#YCM requirements
option(USE_YCM "Generate compiler json for YouCompleteMe" ON)
if (USE_YCM)
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif (USE_YCM)

option(DATALOG_DEBUG "Enable debugging messages for datalog API" ON)
if(DATALOG_DEBUG)
    add_definitions(-DDATALOG_DEBUG)
endif(DATALOG_DEBUG)

option(DATALOG_DEBUG_VERBOSE "Enable verbose debugging messages for datalog API" ON)
if(DATALOG_DEBUG_VERBOSE)
    add_definitions(-DDATALOG_DEBUG_VERBOSE)
endif(DATALOG_DEBUG_VERBOSE)

option(DATALOG_ERR "Enable debugging messages" ON)
if(DATALOG_ERR)
    add_definitions(-DDATALOG_ERR)
endif(DATALOG_ERR)

#includes
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/libs/libdatalog)
include_directories(${PROJECT_SOURCE_DIR}/libs/libdatalog/lua)

#SRCS
#tars
file(GLOB SOURCES "main.c")

#PRINTS
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR} )

#my datalog api
file(GLOB DATALOG_API_SRCS "datalog_api.c")
add_library(DATALOG_API SHARED ${DATALOG_API_SRCS})

#datalog
file(GLOB DATALOG_SRCS "libs/libdatalog/datalog.c")
add_library(DATALOG SHARED ${DATALOG_SRCS})

#lua
file(GLOB LUA_SRCS "libs/libdatalog/lua/*.c")
add_library(LUA SHARED ${LUA_SRCS})

#EXECUTABLES
add_executable(EXAMPLE ${SOURCES})

#LINKING
target_link_libraries(EXAMPLE DATALOG_API)
target_link_libraries(EXAMPLE DATALOG)
target_link_libraries(EXAMPLE LUA)
target_link_libraries(EXAMPLE m)