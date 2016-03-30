CC=gcc
CFLAGS=-std=c99 -Wall

basic.out: main.c
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) $< -S

clean:
	rm -f basic.out main.s
