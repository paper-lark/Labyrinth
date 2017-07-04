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

obj_dir = bin
src_dir = src

ifeq ($(OS), Windows_NT) 
	CURSES_FLAG = -lpdcurses
else
	CURSES_FLAG = -lncurses
endif

all: labyrinth

labyrinth: auxiliary.o gamescene.o list.o main.o menu.o 
	gcc -Wall  -o $(obj_dir)/labyrinth $(obj_dir)/main.o $(obj_dir)/menu.o $(obj_dir)/gamescene.o $(obj_dir)/auxiliary.o $(obj_dir)/list.o $(CURSES_FLAG)

auxiliary.o: $(src_dir)/auxiliary.c $(src_dir)/auxiliary.h
	gcc -c -Wall -o $(obj_dir)/auxiliary.o $(src_dir)/auxiliary.c

gamescene.o: $(src_dir)/gamescene.c $(src_dir)/gamescene.h
	gcc -c -Wall -o $(obj_dir)/gamescene.o $(src_dir)/gamescene.c

list.o: $(src_dir)/list.c $(src_dir)/list.h
	gcc -c -Wall -o $(obj_dir)/list.o $(src_dir)/list.c
	
main.o: $(src_dir)/main.c $(src_dir)/main.h
	gcc -c -Wall -o $(obj_dir)/main.o $(src_dir)/main.c

menu.o: $(src_dir)/menu.c $(src_dir)/menu.h
	gcc -c -Wall -o $(obj_dir)/menu.o $(src_dir)/menu.c

clean:
	rm -f $(obj_dir)/*.o
