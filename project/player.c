#include "Libreria.h"

int main(int argc,const char *args[]){
    struct stato_player giocatore;    
    int i;
    int *ptr;
    /*Definizione stato (Struct)*/
    giocatore.pid=getpid();
    giocatore.giocatore = (atoi(args[0]));
    
    ptr = malloc(sizeof(int)*SO_NUM_P);
    char *args1[1];
    args1[0] = NULL;
    for(i=0 ;i<SO_NUM_P;i++){ 
       switch(ptr[i] = fork()){
           case -1: {
           printf("error\n");
           exit(0);
           }

           case 0: /*Processo figlio*/
            
            /*printf("I=%d, SO_NUM_G: %d, processo n: %d, padre: %d\n",i,SO_NUM_G,getpid(),getppid()); */
            
            execve("./pedina", NULL, NULL);
            exit(1);

            /*default:
            printf("PLAYER ");
            break;*/
       } 
       
    }

    while (wait(NULL) != -1);
}