
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>

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
int casuale();
int main(){
    
    int tot = SO_ROUND_SCORE;
    int max_rand;                                           
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band;
    int i, j=0;
    int tmp= flag;
    key_t key = 12345;
    int shm_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA),0600);
    char *matrix;
    matrix = shmat(shm_id,  NULL, 0);
    
    /*printf("%d\n",flag);*/
    srand(time(NULL));
    for(i=0;i<flag;i++){
    	max_rand = tot-(tmp-1);
    	if(tmp != 1)
        band = casuale(max_rand,1);
    	else band = tot;
        tot= tot - band;
        /*band = *matrix + casuale(0,SO_BASE*SO_ALTEZZA) ;
       for (j;j<SO_BASE*SO_ALTEZZA;j++){
            printf("posizione %d - valore %c", *matrix, &matrix+j);         /*TODO -- Stampa dell'array in memoria condivisa per poter visualizzare le bandierine*/
        /*}*/
    	printf("TOT:%d -- FLAG:%d -- BAND: %d\n",tot,tmp,band);
    	tmp--;
    }

    while (wait(NULL) != -1);
}

int casuale(int a,int b)  
{  
    return rand() % (a - b + 1) + b;
}  