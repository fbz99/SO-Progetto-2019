#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define SO_NUM_G 2
#define SO_NUM_P 10
#define SO_MAX_TIME 3
#define SO_BASE 6
#define SO_ALTEZZA 2
#define SO_FLAG_MIN 5
#define SO_FLAG_MAX 5
#define SO_ROUND_SCORE 10
#define SO_N_MOVES 20
#define SO_MIN_HOLD_NSEC 10000000

struct stato_player{
	int pid;
	char giocatore;
};

struct stato_pedina{
	int pid;
	char padre;
	int mosse;	
};


void stampa_scacchiera(){
	int i,j;
	char matrice[SO_ALTEZZA*SO_BASE];
    char *mtr;                                          
    key_t key = 12345;
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA),0666);
    mtr = &matrice[0]; 
    mtr = shmat(mat_id, NULL, 0);

	/*for(i=0; i<SO_BASE;i++)printf("__");
	printf("\n");
	for(i=0;i<SO_ALTEZZA*SO_BASE;i++){
		if(i%SO_BASE !=0)printf("|%d", matrice[i]);
		else{
		for(j=0;j<SO_BASE;j++){
			printf("__");
		}

		}
		
		}
		
	*/
		for(i=0; i<SO_BASE;i++)printf(" __");
		printf("\n");
		for(i=0;i<SO_ALTEZZA*SO_BASE;i++){
			printf("|%d", matrice[i]);}
	printf("\n");
	}
