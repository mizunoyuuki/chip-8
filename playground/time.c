#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main(){
	printf("time(NULL): %ld\n", time(NULL));
	printf("rand(): %d\n", rand());
	printf("rand(): %d\n", rand());
	printf("rand(): %d\n", rand());
	printf("rand(): %d\n", rand());

	printf("exec srand() !!\n");
	srand(time(NULL));
	printf("rand(): %d\n", rand());
	printf("rand(): %d\n", rand());
	printf("rand(): %d\n", rand());
	printf("rand(): %d\n", rand());
	printf("rand(): %d\n", rand());
	return 0;
}
