CC=gcc
CFLAGS=-std=c99 -Wall

func_ptr.out: func_ptr.c
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) $< -S

clean:
	rm -f func_ptr.out func_ptr.s
