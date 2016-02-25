libPrefix=/usr

CC=gcc

CFLAGS=-c -W -std=gnu99 -g -O0 -fpic

all: database

database: engine.o
	$(CC) engine.o -shared -o database.so -luuid

engine.o: engine.c engine.h error_codes.h
	$(CC) $(CFLAGS) engine.c

install: database.so
	install -m 0755 database.so $(libPrefix)/lib
	install -m 0755 engine.h $(libPrefix)/include
	install -m 0755 error_codes.h $(libPrefix)/include

clean:
	rm database.so *.o