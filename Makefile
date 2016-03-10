CC = gcc -c
LD = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g

build: main 

run: build
	./main
	
main: main.o transform.o
	$(LD) $^ -o main
	
main.o: main.c transform.h
	$(CC) $< $(CFLAGS) -o $@

transform.o: transform.c transform.h
	$(CC) $< $(CFLAGS) -o $@
	
clean:
	rm -f *.o
	rm -f *~
	rm -f main
