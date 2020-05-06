#include "Libreria.h"

int main(int argc,const char *args[]){
	printf("PID1: %d",getpid());
    struct stato_player giocatore;    
    struct msgbuf my_msg;
    int i,rand_pos;
    int *ptr, pos;
    int *old_pos;
    char *args1[2];
    char *matrice; 
    char stringa[10];
    int msg_id;   
    int sem_id_mat, sem_id_mutex, sem_id_zero;
        
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT |0666);
    matrice = shmat(mat_id, NULL, 0);
    
    /*Definizione stato (Struct)*/
    ptr = malloc(sizeof(int)*SO_NUM_P);
    giocatore.pid=getpid();
    giocatore.giocatore =  (atoi(args[0]))+64;

    /*Creazione coda di messaggi*/
    msg_id = msgget(key1, IPC_CREAT | 0666);
    
    sem_id_mutex = semget(key3, 1, IPC_CREAT | 0666);
    reserveSem(sem_id_mutex,0);

    /*Inserimento pedine in posizione casuale*/
    for (pos = 0; pos < SO_NUM_P; pos++) /*SETTAGGIO old pos*/
		old_pos[pos] = 0;
	
    srand(getpid());	/*Aggiunto*/
    for(i=0;i<SO_NUM_P;i++){
        rand_pos = casuale(SO_BASE * SO_ALTEZZA, 0);
        /*printf("Giocatore: %c\n",giocatore.giocatore);*/
        old_pos[i]=rand_pos;
        if(val_check(rand_pos, old_pos) == 1)rand_pos = casuale(SO_BASE * SO_ALTEZZA,0);
        matrice[rand_pos] = giocatore.giocatore;
    }

    args1[1] = NULL;
    for(i=0 ;i<SO_NUM_P;i++){ 
        switch(ptr[i] = fork()){
        case -1:
            printf("error\n");
            exit(0);
            break;

        case 0: /*Processo figlio*/
            itoa(old_pos[i],stringa);
           	copiaArray(my_msg.mtext,stringa);
            msgsnd(msg_id, &my_msg, MSG_LEN, 0);
            execve("./pedina", args1, NULL);
            exit(1);
            break;
        } 
    }
    
    releaseSem(sem_id_mutex,0);
    while (wait(NULL) != -1);
}