.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=client server
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

client:selectclient.o commsocket.o
	$(CC) $(CFLAGS) $^ -o $@

server:selectserver.o commsocket.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o $(BIN)

