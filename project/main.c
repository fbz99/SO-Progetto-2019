#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
       #include <sys/wait.h>


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

    printf("master process: %d\n",getpid());

    for(i=0 ;i<SO_NUM_G;i++,ptr++){ 
       switch(*ptr = fork()){
           case -1: {
           printf("error\n");
           exit(0);
           }

           case 0: /*Processo figlio*/
            //printf("figliotest\n");
            execve("./player", NULL, NULL);
            printf("errore");
            exit(1);

            default:printf("I=%d, SO_NUM_G: %d, processo n: %d, padre: %d\n",i,SO_NUM_G,getpid(),getppid()); break;
       } 
       
    }

    while (wait(NULL) != -1);
}