#include "Libreria.h"

void stampa_scacchiera(int base, int altezza){
	int i, j;
	char *matrice;
	int mat_id = shmget(key, sizeof(int) * (base * altezza), IPC_CREAT | 0666);
	matrice = shmat(mat_id, NULL, 0);
	yellow();

	/*PRIMA RIGA*/
	printf("\n");
	for (i = 1; i <= base; i++){
		if(i == base)
			printf("+--+\n");
		else
			printf("+--");
	}
	
	/*STRUTTURA CENTRALE*/
	for(i=1;i<=base*altezza;i++){
		if(i % base != 0){
			if(matrice[i]==0){
				printf("|  ");
			}
			else{
				if(matrice[i] > 64){
					printf("|");
					blue();
					printf("%c ",matrice[i]);
					yellow();
				}
				else{
					if(matrice[i] < 10){
						printf("|");
						red();
						printf("%d ",matrice[i]);
						yellow();
					}
					else{
						printf("|");
						red();
						printf("%d",matrice[i]);
						yellow();
					}
				}
			}
		}
	/*ULTIMA COLONNA*/
		else if(i % base == 0){
			if(matrice[i]==0)
				printf("|  |\n");
			else{
				if(matrice[i] > 64){
					printf("|");
					blue();
					printf("%c ",matrice[i]);
					yellow();
					printf("|\n");
				}
				else{
					if(matrice[i] < 10){
						printf("|");
						red();
						printf("%d ",matrice[i]);
						yellow();
						printf("|\n");
					}
					else{
						printf("|");
						red();
						printf("%d",matrice[i]);
						yellow();
						printf("|\n");
					}
				}
			}
			for (j = 1; j <= base; j++){
				if (j == base )
					{printf("+--+");}
				else
					printf("+--");
			}
			printf("\n");
		}
	}
	printf("\n");
}

/*Colori Scacchiera*/
void red(){
	printf("\033[1;31m");
}

void yellow(){
	printf("\033[1;33m");
}

void blue(){
	printf("\033[0;34m");
}

void magenta(){
	printf("\033[0;35m");
}

void reset(){
	printf("\033[0m");
}

int sem_set_val(int sem_id, int sem_num, int sem_val){
	return semctl(sem_id, sem_num, SETVAL, sem_val);
}

/*Tento di accedere al semaforo*/
int sem_reserve(int sem_id, int sem_num){
	struct sembuf sops;
	
	sops.sem_num = sem_num;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	return semop(sem_id, &sops, 1);
}

/*Rilascio il semaforo*/
int sem_release(int sem_id, int sem_num){
	struct sembuf sops;
	sops.sem_num = sem_num;
	sops.sem_op = 1;
	sops.sem_flg = 0;
	return semop(sem_id, &sops, 1);
}

int aspetta_zero(int sem_id, int sem_num){
	struct sembuf sops;
	sops.sem_num = sem_num;
	sops.sem_op = 0;
	sops.sem_flg = 0;
	return semop(sem_id, &sops, 1);
}

int casuale(int a, int b){
	return rand() % (a - b + 1) + b;
}

int val_check(int val, int *pos){
	int i;
	int ver = 0;
	size_t n = sizeof(pos) / sizeof(pos[0]);
	for (i = 0; i < n; i++)
		if (pos[i] == val)
			ver = 1;
	return ver;
}

/* Richiesta di accesso a risorsa aspettando per un limite di tempo */
int sem_reserve_wait_time(int sem_id, int sem_num){
	struct sembuf sops;
	struct timespec ts;
	ts.tv_sec = 0;
    ts.tv_nsec = 1000000;

	sops.sem_num = sem_num;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	return semtimedop(sem_id,&sops,1,&ts);
}

calcR(int pos, int col){ /*Calcolo riga data posizione*/
	return pos / col;  
}

calcC(int r, int pos, int col){ /*Calcolo colonna data posizione*/
	return pos - (r * col);
}

void config_var_cond(){ /*Inserimento variabili condivise in array*/
	struct variabiliCondivise *varCond;
	int i = 0;
	int shVar_id;
	int *var;
	FILE *fp;

	var = malloc(sizeof(int)*10);
	fp = fopen("Variabili.conf", "r");
	fscanf(fp, "%*s\t%*c\t%d\n%*s\t%*c\t%d\n%*s\t%*c\t%d\n%*s\t%*c\t%d\n%*s\t%*c\t%d\n%*s\t%*c\t%d\n%*s\t%*c\t%d\n%*s\t%*c\t%d\n%*s\t%*c\t%d\n%*s\t%*c\t%d\n",
		&var[0],&var[1],&var[2],&var[3],&var[4],&var[5],&var[6],&var[7],&var[8],&var[9]);
	fclose(fp);

	shVar_id = shmget(key6, sizeof(int)*10, IPC_CREAT | 0666);
	varCond = shmat(shVar_id, NULL, 0);
	
	varCond->SO_NUM_G = var[0];
	varCond->SO_NUM_P = var[1];
	varCond->SO_MAX_TIME = var[2];
	varCond->SO_BASE = var[3];
	varCond->SO_ALTEZZA = var[4];
	varCond->SO_FLAG_MIN = var[5];
	varCond->SO_FLAG_MAX = var[6];
	varCond->SO_ROUND_SCORE = var[7];
	varCond->SO_N_MOVES = var[8];
	varCond->SO_MIN_HOLD_NSEC = var[9];
}
