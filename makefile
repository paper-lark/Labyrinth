obj_dir = bin
src_dir = src

ifeq ($(OS), Windows_NT) 
	CURFLAGS = -lpdcurses
else
	CURFLAGS = -lncurses
endif

all: labyrinth.o stack.o
	gcc -Wall $(obj_dir)/labyrinth.o $(obj_dir)/stack.o -o $(obj_dir)/labyrinth $(CURFLAGS)

labyrinth.o: $(src_dir)/labyrinth.c $(src_dir)/stack.h
	gcc -c -Wall -o $(obj_dir)/labyrinth.o $(src_dir)/labyrinth.c

stack.o: $(src_dir)/stack.c $(src_dir)/stack.h
	gcc -c -Wall -o $(obj_dir)/stack.o $(src_dir)/stack.c

clean:
	rm -f $(obj_dir)/*
