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
	CURFLAG = -lpdcurses
else
	CURFLAG = -lncurses
endif

all: labyrinth.o list.o auxiliary.o
	gcc -Wall $(obj_dir)/labyrinth.o $(obj_dir)/auxiliary.o $(obj_dir)/list.o -o $(obj_dir)/labyrinth $(CURFLAG)

labyrinth.o: $(src_dir)/labyrinth.c $(src_dir)/list.h
	gcc -c -Wall -o $(obj_dir)/labyrinth.o $(src_dir)/labyrinth.c

list.o: $(src_dir)/list.c $(src_dir)/list.h
	gcc -c -Wall -o $(obj_dir)/list.o $(src_dir)/list.c

auxiliary.o: $(src_dir)/auxiliary.c $(src_dir)/auxiliary.h
	gcc -c -Wall -o $(obj_dir)/auxiliary.o $(src_dir)/auxiliary.c

clean:
	rm -f $(obj_dir)/*
