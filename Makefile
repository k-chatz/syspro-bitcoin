output: main.o bitcoin.o hash.o list.o transaction.o wallet.o
   gcc main.o bitcoin.o hash.o list.o transaction.o wallet.o -o bitcoin

main.o: main.c
   gcc: -c main.c

bitcoin.o: bitcoin.c bitcoin.h
   gcc -c bitcoin.c

hash.o: hash.c hash.h
   gcc -c hash.c

list.o: list.c list.h
   gcc -c list.c

transaction.o: transaction.c transaction.h
   gcc -c transaction.c

wallet.o: wallet.c wallet.h
   gcc -c wallet.c

clean:
    rm *.o output
