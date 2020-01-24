
#include "Libreria.h"

int casuale();
int val_check();

int main(){    
    int tot = SO_ROUND_SCORE;
    int max_rand, size = SO_ALTEZZA*SO_BASE ;
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band, i, j, pos, rand_pos;
    int *old_pos;
    int tmp = flag;
    key_t key = 12345;
    char *matrice;        
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);
    
    old_pos = malloc(sizeof(int)*flag);
    for(i=0;i<SO_ALTEZZA*SO_BASE;i++) matrice[i] = '0';

    
    srand(time(NULL));
    for(i=0;i<flag;i++){
        
        rand_pos = casuale(size, 0);
        old_pos[i]=rand_pos;
        if(val_check(rand_pos, old_pos) == 1)rand_pos = casuale(size,0);
        
    	max_rand = tot-(tmp-1);
    	if(tmp != 1){
            band = casuale(max_rand,1);
        }
    	else band = tot;
        tot = tot - band;
        
        if(band!=0)
        matrice[rand_pos] = band;
        
    	tmp--;
        
    }
    while (wait(NULL) != -1);
}