OBJS	= main.o bitcoin.o hash.o list.o transaction.o wallet.o
SOURCE	= main.c bitcoin.c hash.c list.c transaction.c wallet.c
HEADER	= bitcoin.h hash.h list.h transaction.h wallet.h
OUT	= bitcoin
CC	 = gcc
FLAGS	 = -c -Wall

all: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)

main.o: main.c
	$(CC) $(FLAGS) main.c

bitcoin.o: bitcoin.c
	$(CC) $(FLAGS) bitcoin.c

hash.o: hash.c
	$(CC) $(FLAGS) hash.c

list.o: list.c
	$(CC) $(FLAGS) list.c

transaction.o: transaction.c
	$(CC) $(FLAGS) transaction.c

wallet.o: wallet.c
	$(CC) $(FLAGS) wallet.c

clean:
	rm -f $(OBJS) $(OUT)
