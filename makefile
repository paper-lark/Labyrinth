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

# Variables
OBJ_DIR = bin
SRC_DIR = src

ifeq ($(OS), Windows_NT) 
	LIBFLAGS = ws2_32.lib pdcurses.lib
	OBJFLAGS = /c /Wall /Fo:
	EXECFLAGS = /Wall /Fe:
	COMPILER = cl
	DELETE = del
	LIB_DIR = win
else
	COMPILER = gcc
	DELETE = rm
	OBJFLAGS = -Wall -c -o
	EXECFLAGS = -Wall -o
	LIBFLAGS = -lncurses
	LIB_DIR = unix
endif


# Targets
.PHONY: all clean

all: $(OBJ_DIR)/labyrinth

$(OBJ_DIR)/labyrinth: $(OBJ_DIR)/auxiliary.o $(OBJ_DIR)/gamescene.o $(OBJ_DIR)/list.o $(OBJ_DIR)/main.o $(OBJ_DIR)/menu.o $(OBJ_DIR)/connect.o $(OBJ_DIR)/transmit.o
	$(COMPILER) $(EXECFLAGS) $(OBJ_DIR)/labyrinth $(OBJ_DIR)/main.o $(OBJ_DIR)/menu.o $(OBJ_DIR)/gamescene.o $(OBJ_DIR)/connect.o $(OBJ_DIR)/transmit.o $(OBJ_DIR)/auxiliary.o $(OBJ_DIR)/list.o $(LIBFLAGS)

$(OBJ_DIR)/auxiliary.o: $(SRC_DIR)/auxiliary.c $(SRC_DIR)/auxiliary.h
	$(COMPILER) $(OBJFLAGS) $(OBJ_DIR)/auxiliary.o $(SRC_DIR)/auxiliary.c

$(OBJ_DIR)/gamescene.o: $(SRC_DIR)/gamescene.c $(SRC_DIR)/gamescene.h
	$(COMPILER) $(OBJFLAGS) $(OBJ_DIR)/gamescene.o $(SRC_DIR)/gamescene.c

$(OBJ_DIR)/list.o: $(SRC_DIR)/list.c $(SRC_DIR)/list.h
	$(COMPILER) $(OBJFLAGS) $(OBJ_DIR)/list.o $(SRC_DIR)/list.c
	
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/main.h
	$(COMPILER) $(OBJFLAGS) $(OBJ_DIR)/main.o $(SRC_DIR)/main.c

$(OBJ_DIR)/menu.o: $(SRC_DIR)/menu.c $(SRC_DIR)/menu.h
	$(COMPILER) $(OBJFLAGS) $(OBJ_DIR)/menu.o $(SRC_DIR)/menu.c

$(OBJ_DIR)/connect.o: $(SRC_DIR)/$(LIB_DIR)/connect.c $(SRC_DIR)/connect.h
	$(COMPILER) $(OBJFLAGS) $(OBJ_DIR)/connect.o $(SRC_DIR)/$(LIB_DIR)/connect.c 

$(OBJ_DIR)/transmit.o: $(SRC_DIR)/transmit.c $(SRC_DIR)/transmit.h
	$(COMPILER) $(OBJFLAGS) $(OBJ_DIR)/transmit.o $(SRC_DIR)/transmit.c 

clean:
	$(DELETE) $(OBJ_DIR)/*.o $(OBJ_DIR)/*.obj
