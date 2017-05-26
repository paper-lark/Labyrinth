obj_dir = bin
src_dir = src

ifeq ($(OS), Windows_NT) 
	CURFLAG = -lpdcurses
else
	CURFLAG = -lncurses
endif

all: labyrinth.o list.o aux.o
	gcc -Wall $(obj_dir)/labyrinth.o $(obj_dir)/aux.o $(obj_dir)/list.o -o $(obj_dir)/labyrinth $(CURFLAG)

labyrinth.o: $(src_dir)/labyrinth.c $(src_dir)/list.h
	gcc -c -Wall -o $(obj_dir)/labyrinth.o $(src_dir)/labyrinth.c

list.o: $(src_dir)/list.c $(src_dir)/list.h
	gcc -c -Wall -o $(obj_dir)/list.o $(src_dir)/list.c

aux.o: $(src_dir)/aux.c $(src_dir)/aux.h
	gcc -c -Wall -o $(obj_dir)/aux.o $(src_dir)/aux.c

clean:
	rm -f $(obj_dir)/*
