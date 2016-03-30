CC=gcc
CFLAGS=-std=c99 -Wall

func_ptr: func_ptr.c
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) $< -S

clean:
	rm -f func_ptr func_ptr.s
