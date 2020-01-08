#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define SO_NUM_G 2
#define SO_NUM_P 10
#define SO_MAX_TIME 3
#define SO_BASE 60
#define SO_ALTEZZA 20
#define SO_FLAG_MIN 5
#define SO_FLAG_MAX 5
#define SO_ROUND_SCORE 10
#define SO_N_MOVES 20
#define SO_MIN_HOLD_NSEC 10000000

int main(){
	int i,j;

	for(i=0;i<SO_BASE;i++){
		printf("__");
	}
	printf("\n");
	for(i=0;i<SO_ALTEZZA;i++){
		for(j=0;j<SO_BASE;j++){
			printf("|0");
		}
		printf("|\n");
		for(j=0;j<SO_BASE;j++){
			printf("__");
		}
		printf("\n");
	}
}