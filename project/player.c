#include "Libreria.h"

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

int main(int argc,const char *args[]){
    struct stato_player giocatore;    
    int i;
    pid_t pedine[SO_NUM_P];
    int *ptr = pedine;

    /*Definizione stato (Struct)*/
    giocatore.pid=getpid();
    giocatore.giocatore = (atoi(args[0]));
    printf("Stampa giocatore struct: %d\n",giocatore.giocatore);
    for(i=0 ;i<SO_NUM_P;i++,ptr++){ 
       switch(*ptr = fork()){
           case -1: {
           printf("error\n");
           exit(0);
           }

           case 0: /*Processo figlio*/
            execve("./pedine", NULL, NULL);
            printf("I=%d, SO_NUM_G: %d, processo n: %d, padre: %d\n",i,SO_NUM_G,getpid(),getppid()); 
            exit(1);

            default:
            /*printf("PLAYER ");*/
            break;
       } 
       
    }

    while (wait(NULL) != -1);
}