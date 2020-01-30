#include "Libreria.h"

int main(int argc,const char *args[]){
	struct stato_pedina pedina;
	char *matrice; 
	int sem_id_mat;
	int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT |0666);
    matrice = shmat(mat_id, NULL, 0);
    pedina.pid = getpid();
    pedina.pos = atoi(args[0]);

    sem_id_mat= semget (key2, SO_BASE*SO_ALTEZZA, IPC_CREAT | 0666);
    printf("PID pedina: %d - Pos: %d\n",pedina.pid, pedina.pos);
	
}