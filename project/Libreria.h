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
    char *matrice;                                          
    key_t key = 12345;
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);

	/*PRIMA RIGA*/	
	for(i=0; i<SO_BASE;i++){
		if(i==SO_BASE-1)printf("+\n");
		else printf("+--");
	}
	
	/*STRUTTURA CENTRALE*/
	for(i=1;i<=SO_ALTEZZA*SO_BASE;i++){
	
	   	if(i%SO_BASE != 0){
		   	if(matrice[i] == '0')printf("|  ",matrice[i]);
			else printf("| %d",matrice[i]);
		}
				
		else if(i%SO_BASE == 0){	
			printf("|\n");
			for(j=0; j<SO_BASE;j++){
				if(j==SO_BASE-1)printf("+\n");
				else printf("+--");
			}
		} 
	}
	printf("\n");
	
}