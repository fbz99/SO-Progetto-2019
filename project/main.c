#include "Libreria.h"

int casuale();
int main(){
    int i, size = SO_ALTEZZA*SO_BASE;
    pid_t players[SO_NUM_G];
    int *ptr = players;
    int flag_n = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN; 
    key_t key = 12345;
    int tot = SO_ROUND_SCORE;
    int max_rand;                                           
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band, j=0, rand_pos, pos;
    int tmp = flag;
    char * args [2]; 
    char matrice[SO_ALTEZZA*SO_BASE];
    char *mtr;            
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA),0666);
    mtr = &matrice[0]; 
    mtr = shmat(mat_id, NULL, 0);

       	for (pos = 0; pos < size; pos++) /*SETTAGGIO MATRICE*/
		matrice[pos] = 0;
    printf("Master process: %d\n",getpid());


    /*Creazione Giocatori*/
    for(i=0 ;i<SO_NUM_G;i++,ptr++){ 
       switch(*ptr = fork()){
           case -1: {
           printf("Error\n");
           exit(0);
           }

           case 0: /*Processo figlio*/
            *args[0]= (char)i;
            printf("PID Giocatore : %d\n", getpid());
            execve("./player", args, NULL);
            exit(1);
            break;

            default: printf("default fork\n");
            
            break;
       } 
       
    }

    execve("./bandierina",NULL,NULL);


    while (wait(NULL) != -1);
}

int casuale(int a,int b)  
{  
    return rand() % (b - a + 1) + a;
}  