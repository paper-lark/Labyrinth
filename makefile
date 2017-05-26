obj_dir = bin
src_dir = src

ifeq ($(OS), Windows_NT) 
	CURFLAG = -lpdcurses
else
	CURFLAG = -lncurses
endif

all: labyrinth.o list.o
	gcc -Wall $(obj_dir)/labyrinth.o $(obj_dir)/list.o -o $(obj_dir)/labyrinth $(CURFLAG)

labyrinth.o: $(src_dir)/labyrinth.c $(src_dir)/list.h
	gcc -c -Wall -o $(obj_dir)/labyrinth.o $(src_dir)/labyrinth.c

list.o: $(src_dir)/list.c $(src_dir)/list.h
	gcc -c -Wall -o $(obj_dir)/list.o $(src_dir)/list.c

clean:
	rm -f $(obj_dir)/*
