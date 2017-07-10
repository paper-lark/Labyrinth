##################################################################################
# Copyright (c) 2017 Max Zhuravsky												 #
#                                                                                #
# Permission is hereby granted, free of charge, to any person obtaining a copy   #
# of this software and associated documentation files (the "Software"), to deal  #
# in the Software without restriction, including without limitation the rights   #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      #
# copies of the Software, and to permit persons to whom the Software is          #
# furnished to do so, subject to the following conditions:                       #
#                                                                                #
# The above copyright notice and this permission notice shall be included in all #
# copies or substantial portions of the Software.                                #
#                                                                                #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  #
# SOFTWARE.                                                                      #
##################################################################################

##################################################################################
#                                 Labyrinth Game                                 #
#                  Author:  Max Zhuravsky <paperlark@yandex.ru>                  #
##################################################################################

# Project
cmake_minimum_required (VERSION 3.0)
project(Labyrinth C)

# Version
set(Labyrinth_VERSION_MAJOR 3)
set(Labyrinth_VERSION_MINOR 0)
set(Labyrinth_BUILD_TYPE "Prerelease")

# Configure paths
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(CMAKE_CURRENT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
configure_file("${PROJECT_SOURCE_DIR}/main.h.in" 
               "${PROJECT_BINARY_DIR}/include/main.h")
include_directories("${PROJECT_BINARY_DIR}/include")

# Find dependencies
if(WIN32)
    set(LIBRARIES ${LIBRARIES} ws2_32)
    set(LIBRARIES ${LIBRARIES} pdcurses)
else()
    find_package(Curses REQUIRED)
    if(NOT Curses_FOUND)
	    message(SEND_ERROR "Failed to find Curses")
	    return()
    else()
	    include_directories(${Curses_INCLUDE_DIR})
        set(LIBRARIES ${LIBRARIES} ${CURSES_LIBRARIES})
    endif()
endif()

# Build
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
if (NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types-discards-qualifiers")
endif()

set(SRC_FILES ${PROJECT_SOURCE_DIR}/main.c ${PROJECT_SOURCE_DIR}/menu.c ${PROJECT_SOURCE_DIR}/gamescene.c ${PROJECT_SOURCE_DIR}/transmit.c ${PROJECT_SOURCE_DIR}/auxiliary.c ${PROJECT_SOURCE_DIR}/list.c)
if(WIN32)
    set(SRC_FILES ${SRC_FILES} ${PROJECT_SOURCE_DIR}/win/connect.c)
else()
    set(SRC_FILES ${SRC_FILES} ${PROJECT_SOURCE_DIR}/unix/connect.c)
endif()
add_executable(labyrinth ${SRC_FILES})

message(STATUS "Executable is in ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "Linking following libraries: ${LIBRARIES} ...")

target_link_libraries(labyrinth ${LIBRARIES})

