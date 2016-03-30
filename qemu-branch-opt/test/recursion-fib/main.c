#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long F(int n) {
	if (n <= 1)	return 1;
	return F(n-1) + F(n-2);
}
int main(int argc, char *argv[]) {
	if (argc != 2) {
		puts("Usage: <Test>");
		exit(1);
	}
	int n = atoi(argv[1]);
	long long fn = F(n);
	printf("Fib(%d) = %lld\n", n, fn);
	return 0;
}
