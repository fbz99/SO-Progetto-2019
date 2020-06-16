#include "Libreria.h"

int main(int argc, const char *args[])
{

    char *matrice;
    int sem_id_mat,sem_id_ped;
    int mat_id = shmget(key, sizeof(int) * (SO_BASE * SO_ALTEZZA), IPC_CREAT | 0666);
    struct stato_pedina pedina;
    /*int cont = atoi(args[1]);*/
    sem_id_mat = semget(key2, SO_BASE * SO_ALTEZZA, IPC_CREAT | 0666);
    sem_id_ped = semget(key4, SO_NUM_P, IPC_CREAT | 0666);
    printf("args - %d", args[1]);
    pedina.pid = getpid();
    pedina.pos = atoi(args[0]);
    matrice = shmat(mat_id, NULL, 0);
    reserveSem(sem_id_mat, pedina.pos);
    /*reserveSem(sem_id_ped,1);*/
    /*printf("CONTATORE: %d\n",cont);*/

    /*printf("PID pedina: %d - Pos: %d\n", pedina.pid, pedina.pos);*/
    while (wait(NULL) != -1);
    releaseSem(sem_id_mat,pedina.pos);
    /*releaseSem(sem_id_ped,cont);*/
}