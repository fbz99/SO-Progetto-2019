#include "Libreria.h"

int main(){
	key_t key = 12345;
	struct stato_pedina pedina;
	char *matrice; 
	int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT |0666);
    matrice = shmat(mat_id, NULL, 0);
    pedina.pid = getpid();
	
}