obj_dir = bin
src_dir = src

ifeq ($(OS), Windows_NT) 
	CFLAGS = -lpdcurses
else
	CFLAGS = -lncurses
endif

all: test.o
	gcc -Wall $(obj_dir)/test.o -o $(obj_dir)/test $(CFLAGS)

test.o: $(src_dir)/test.c
	gcc -c -Wall -o $(obj_dir)/test.o $(src_dir)/test.c

clean:
	rm -f $(obj_dir)/*
