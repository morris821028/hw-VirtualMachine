CC=gcc
CFLAGS=-std=c99 -Wall

fib.out: main.c
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) $< -S

clean:
	rm -f fib.out main.s
