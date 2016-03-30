#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int route_A(int n) {
	int sum = 0;	
	for (int i = 0; i < n; i++)
		sum += i;
	return sum;
}
int route_B(int n) {
	int sum = 0;
	for (int i = 0; i < n; i++)
		sum += i*i;
	return sum;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		puts("Usage: <Test>");
		exit(1);
	}
	int (*ROUTE[])(int) = {route_A, route_B};
	int route_n = sizeof(ROUTE)/sizeof(ROUTE[0]);
	int n = atoi(argv[1]);
	unsigned int checksum = 0;
	// srand(time(NULL));
	printf("#Route = %d, run %d\n", route_n, n);
	for (int i = 0; i < n; i++) {
		int c = rand()&1;
		switch (c) {
			case 0:	checksum += (*(ROUTE[0]))(5); break;
			case 1: checksum += (*(ROUTE[1]))(5); break;
		}
	}
	printf("Checksum %X\n", checksum);
	return 0;
}
