libPrefix=/usr

CC=gcc

CFLAGS=-c -W -std=gnu99 -g -O0 -fpic

all: database

database: engine.o
	$(CC) engine.o -shared -o libdatabase.so -luuid

engine.o: engine.c engine.h error_codes.h
	$(CC) $(CFLAGS) engine.c

install: libdatabase.so
	install -m 0755 libdatabase.so $(libPrefix)/lib
	install -m 0755 engine.h $(libPrefix)/include
	install -m 0755 error_codes.h $(libPrefix)/include
	install -m 0755 database.h $(libPrefix)/include

uninstall:
	rm $(libPrefix)/lib/libdatabase.so
	rm $(libPrefix)/include/engine.h
	rm $(libPrefix)/include/error_codes.h
	rm $(libPrefix)/include/database.h

clean:
	rm libdatabase.so *.o