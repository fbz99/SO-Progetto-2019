
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

int main(){
    
    int tot = SO_ROUND_SCORE;
    int max_rand;                                           
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band;
    int i;
    int tmp= flag;

    /*printf("%d\n",flag);*/
    srand(time(NULL));
    for(i=0;i<flag;i++){
    	max_rand = tot-(tmp-1);
    	band = (rand() % (max_rand))+1;
    	tot= tot - band;
    	printf("TOT:%d -- FLAG:%d -- BAND: %d\n",tot,tmp,band);
    	tmp--;
    }

}