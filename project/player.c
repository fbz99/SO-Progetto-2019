#include "Libreria.h"

int main(int argc,const char *args[]){
    struct stato_player giocatore;    
    int i;
    int *ptr;
    /*Definizione stato (Struct)*/
    /*giocatore.pid=getpid();*/
    giocatore.giocatore = (atoi(args[0]));
    printf("Giocatore in esecuzione: %d\n",giocatore.giocatore);

    ptr = malloc(sizeof(int)*SO_NUM_P);
    for(i=0 ;i<10;i++){ 
       switch(ptr[i] = fork()){
           case -1: {
           printf("error\n");
           exit(0);
           }

           case 0: /*Processo figlio*/
            printf("I=%d, SO_NUM_G: %d, processo n: %d, padre: %d\n",i,SO_NUM_G,getpid(),getppid()); 
            exit(1);
            /*if (execve("./pedine", NULL, NULL) == -1) printf("(diofa)\n");*/
          
       } 
       
    }

    while (wait(NULL) != -1);
}