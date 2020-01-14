
#include "Libreria.h"

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
int casuale();
int main(){
    
    int tot = SO_ROUND_SCORE;
    int max_rand, size = SO_ALTEZZA*SO_BASE ;
    char matrice[SO_ALTEZZA*SO_BASE];
    char *mtr;                                          
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band, i, j, pos, rand_pos;
    int tmp = flag;
    key_t key = 12345;
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA),0666);
    mtr = &matrice[0]; 
    mtr = shmat(mat_id, NULL, 0);
    
    for(i=0;i<SO_ALTEZZA*SO_BASE;i++) matrice[i] = '0';

    
    srand(time(NULL));
    printf("flag: %d  --  tmp: %d\n",flag,tmp);
    for(i=0;i<5;i++){
        rand_pos = casuale(size, 0);
        printf("RAND_POS: %d\n", rand_pos);
    	max_rand = tot-(tmp-1);
    	if(tmp != 1)
        band = casuale(max_rand,1);
    	else band = tot;
        tot = tot - band;
        matrice[rand_pos] = band;
        
    	tmp--;
        printf("TOT:%d -- TMP:%d -- BAND: %d\n",tot,tmp,band);
    }
for(i=0;i<SO_ALTEZZA*SO_BASE;i++){
			printf("|%d", matrice[i]);}
stampa_scacchiera();
    /*
for (j=0;j<size;j++){
            printf("matrice[%d] - %d\n", j, matrice[j]);         
        }*/
    while (wait(NULL) != -1);
}

int casuale(int a,int b)  
{  
    return rand() % (a - b + 1) + b;
}  