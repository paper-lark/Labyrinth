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
set(Labyrinth_BUILD_TYPE "Release")

# Configure project paths
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
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

# Configure compiler flags
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

# Build program
set(SRC_FILES ${PROJECT_SOURCE_DIR}/main.c ${PROJECT_SOURCE_DIR}/menu.c ${PROJECT_SOURCE_DIR}/gamescene.c ${PROJECT_SOURCE_DIR}/transmit.c ${PROJECT_SOURCE_DIR}/auxiliary.c)
if(WIN32)
    set(SRC_FILES ${SRC_FILES} ${PROJECT_SOURCE_DIR}/win/connect.c)
else()
    set(SRC_FILES ${SRC_FILES} ${PROJECT_SOURCE_DIR}/unix/connect.c)
endif()
add_executable(labyrinth ${SRC_FILES})
add_library(list STATIC ${PROJECT_SOURCE_DIR}/list.c)
set(LIBRARIES ${LIBRARIES} list)
target_link_libraries(labyrinth ${LIBRARIES})

# Messages
message(STATUS "Executables will be in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Static libraries will be in ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "Module libraries will be in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "Following libraries linked: ${LIBRARIES} ...")

