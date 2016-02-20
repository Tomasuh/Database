CC=gcc

CFLAGS=-c -W -std=gnu99 -g -O0

all: database

database: engine.o
	$(CC) engine.o -o database -luuid

engine.o: engine.c engine.h error_codes.h
	$(CC) $(CFLAGS) engine.c

clean:
	rm database *.o