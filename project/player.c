#include "Libreria.h"

int main(int argc,const char *args[]){
    struct stato_player giocatore;    
    int i,key0 = 44245, rand_pos, key = 12345;
    int *ptr;
    int *old_pos;
    char *args1[1];
    char *matrice;    
    int sem_id_mat, sem_id_mutex, sem_id_zero;     
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT |0666);
    matrice = shmat(mat_id, NULL, 0);
    /*Definizione stato (Struct)*/
    giocatore.pid=getpid();
    giocatore.giocatore = (atoi(args[0]));

    /*sem_id_zero = semget(key0,4, IPC_CREAT | 0666);
    */
    ptr = malloc(sizeof(int)*SO_NUM_P);
    args1[0] = NULL;
    for(i=0 ;i<SO_NUM_P;i++){ 
       switch(ptr[i] = fork()){
           case -1: {
           printf("error\n");
           exit(0);
           }

           case 0: /*Processo figlio*/
            
            /*printf("I=%d, SO_NUM_G: %d, processo n: %d, padre: %d\n",i,SO_NUM_G,getpid(),getppid()); */
            printf("PID GIOCATORE: %d",giocatore.giocatore);
            execve("./pedina", NULL, NULL);
            exit(1);

            default:
            printf("%d, %d\n", ptr[i],getpid());
            break;
       } 
       
    }
    /*Inserimento pedine in posizione casuale*/
    old_pos = malloc(sizeof(int)*SO_NUM_P);
    for(i=0;i<SO_NUM_P;i++){
        rand_pos = casuale(SO_NUM_G*SO_NUM_P, 0);
        old_pos[i]=rand_pos;
        if(val_check(rand_pos, old_pos) == 1)rand_pos = casuale(SO_NUM_P*SO_NUM_G,0);
        matrice[rand_pos] = giocatore.giocatore;
    }
    sem_reserve(sem_id_zero,0);
   stampa_scacchiera();
    while (wait(NULL) != -1);
}