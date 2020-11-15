CC = gcc
CFLAGS = -Wall -I ../include

ts: ../src/*.c
	$(CC) $(CFLAGS) -o mhts ../src/mhts.c ../src/encryption.c -lpthread
	$(CC) $(CFLAGS) -o mhtc ../src/mhtc.c ../src/encryption.c -lpthread

mhtc: ../src/*.c
	$(CC) $(CFLAGS) -o mhtc ../src/tcpc.c ../src/encryption.c

clean: 
	rm tcpc tcps mhtc mhts serv