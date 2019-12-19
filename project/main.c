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
    int i;
    pid_t players[SO_NUM_G];
    int *ptr = players;
    char *matrix;
    int flag_n = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN; 
    key_t key = 12345;

    int shm_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA),0600);
    matrix = shmat(shm_id, NULL, 0);

    printf("Master process: %d\n",getpid());


    /*Creazione Giocatori*/
    for(i=0 ;i<SO_NUM_G;i++,ptr++){ 
       switch(*ptr = fork()){
           case -1: {
           printf("Error\n");
           exit(0);
           }

           case 0: /*Processo figlio*/
            printf("PID Giocatore : %d\n", getpid());
            execve("./player", NULL, NULL);
            exit(1);
            break;

            default: break;
       } 
       
    }

    execve("./bandierina",NULL,NULL);	/*Riporta al file in cui vengono create le bandierine*/

    while (wait(NULL) != -1);
}