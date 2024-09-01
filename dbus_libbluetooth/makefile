CC = gcc
LNK = gcc

CFLAGS = `pkg-config --cflags gio-2.0` -c
LIBDIR = /usr/lib/x86_64-linux-gnu/
LIBS = `pkg-config --libs gio-2.0` -lbluetooth

all: bluetooth

# link using explicit arguments for testing, alternatively simply use
# the flags as provided by pkg-config, i.e. something like that:
# gcc 'pkg-config --libs gio-2.0'

bluetooth: bluetooth.o
	$(LNK) -L$(LIBDIR) bluetooth.o -o bluetooth $(LIBS)

# compile using explicit arguments for testing, alternatively simply use
# the flags as provided by pkg-config, i.e. something like that:
# gcc 'pkg-config --cflags gio-2.0'

bluetooth.o: bluetooth.c
	$(CC) ${CFLAGS} bluetooth.c

clean:
	rm -rf bluetooth bluetooth.o
