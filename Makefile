CC=gcc
CFLAGS=-I.

assign4: main.c
	$(CC) -o assign4 main.c $(CFLAGS)

clean:
	rm assign4 *.o