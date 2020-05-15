#include "Libreria.h"

int main(int argc, const char *args[])
{
    /*printf("PID1: %d\n", getpid());*/
    struct stato_player giocatore;
    struct msgbuf my_msg;
    int i, rand_pos, j = 0;
    int *ptr, pos;
    int *old_pos;
    char *args1[2];
    char *matrice;
    char stringa[10];
    int msg_id;
    int sem_id_mat, sem_id_mutex, sem_id_zero;

    /*char *my_messaggio;
    printf(" sizeof char %ld", sizeof(my_messaggio));*/
    int mat_id = shmget(key, sizeof(int) * (SO_BASE * SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);

    /*Definizione stato (Struct)*/
    ptr = malloc(sizeof(int) * SO_NUM_P);
    giocatore.pid = getpid();
    giocatore.giocatore = (atoi(args[0])) + 64;

    /*Creazione coda di messaggi*/
    msg_id = msgget(key_coda, IPC_CREAT | 0666);

    sem_id_mutex = semget(key3, 1, IPC_CREAT | 0666);
    reserveSem(sem_id_mutex, 0);

    /*Inserimento pedine in posizione casuale*/
    for (pos = 0; pos < SO_NUM_P; pos++) /*SETTAGGIO old pos*/
        old_pos[pos] = 0;

    srand(getpid());

    args1[1] = NULL;
    for (i = 0; i < SO_NUM_P; i++)
    {
        rand_pos = casuale(SO_BASE * SO_ALTEZZA, 1);
        while (semctl(sem_id_mat, rand_pos, GETVAL) != 1)
        {
            /*printf("%d\n",semctl(sem_id_mat,rand_pos,GETVAL));*/
            rand_pos = casuale(SO_BASE * SO_ALTEZZA, 0);
        }
        old_pos[i] = rand_pos;

        sprintf(stringa, "%d", rand_pos);
        args1[0] = stringa;
        matrice[rand_pos] = giocatore.giocatore;
       /* printf("Rand_pos: %d - matrice[rand_pos]: %c\n", rand_pos,matrice[rand_pos]);*/
       /* printf("Giocatore: %c\n", giocatore.giocatore);*/
       
        switch (ptr[i] = fork())
        {
        case -1:
            printf("error\n");
            exit(0);
            break;

        case 0: /*Processo figlio*/
            /*itoa(old_pos[i],stringa);
          	copiaArray(my_msg.mtext,stringa);
            msgsnd(msg_id, &my_msg, MSG_LEN, 0);  /*Invio del messaggio nella coda*/
            execve("./pedina", args1, NULL);
            exit(1);
            break;
        }
    }
     /*stampaArray();*/
    
    releaseSem(sem_id_mutex, 0);
    while (wait(NULL) != -1);
}