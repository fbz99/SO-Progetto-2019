#include "Libreria.h"

int main(int argc, const char *args[])
{
    char *matrice;
    int sem_id_mat;
    int mat_id = shmget(key, sizeof(int) * (SO_BASE * SO_ALTEZZA), IPC_CREAT | 0666);
    struct stato_pedina pedina;
    sem_id_mat = semget(key2, SO_BASE * SO_ALTEZZA, IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);
    pedina.pid = getpid();
    pedina.pos = atoi(args[0]);
    printf("Panino - PID:%d \n",getpid());
    reserveSem(sem_id_mat, pedina.pos);

    printf("PID pedina: %d - Pos: %d\n", pedina.pid, pedina.pos);
}