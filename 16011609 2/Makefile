CC = gcc
CFLAGS = -O2 -Wall -pedantic

all: client server

client: client.o
	$(CC) $(CFLAGS) $(LIBS) -o client client.o

server: server.o
	$(CC) $(CFLAGS) $(LIBS) -o server server.o

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -f *.o
