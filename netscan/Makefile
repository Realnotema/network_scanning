CC=gcc
CFLAGS=-c -Wall

all: main.o netscan.o
	$(CC) main.o netscan.o -o netscan

main.o: main.c
	$(CC) $(CFLAGS) main.c

netscan.o: netscan.c
	$(CC) $(CFLAGS) netscan.c
