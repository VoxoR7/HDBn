CC=g++
CFLAGS=-g

PROG=main

$(PROG): HDBn.o main.o
	$(CC) main.o HDBn.o -o $(PROG)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

HDBn.o: HDBn.cpp HDBn.h
	$(CC) $(CFLAGS) -c HDBn.cpp -o HDBn.o

clean:
	rm -f *.o

cleanmake: clean
	clear
	make

make: $(PROG)