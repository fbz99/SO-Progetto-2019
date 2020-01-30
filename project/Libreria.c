#include "Libreria.h"

void stampa_scacchiera(){
	int i,j;
    char *matrice;                                          
    key_t key = 12345;
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);

		
	
	yellow();
	/*PRIMA RIGA*/
	for(i=0; i<SO_BASE;i++){
		if(i==SO_BASE-1)printf("+\n");
		else printf("+---");
	}
	
	/*STRUTTURA CENTRALE*/
	for(i=1;i<=SO_ALTEZZA*SO_BASE;i++){
	
	   	if(i%SO_BASE != 0){
		   	if(matrice[i] == '0')printf("|   ");
			else {
               				printf("| ");
			if(matrice[i] >= '0' && matrice[i]<='9'){
			reset();
			
			} 
			else if(matrice[i] >= 'A'- 1 && matrice[i]<='Z') {
			reset();
				blue();
			}

			printf(" %c",matrice[i]);
		yellow();
            }
		}
				
		else if(i%SO_BASE == 0){	
			printf("|\n");
			for(j=0; j<SO_BASE;j++){
				if(j==SO_BASE-1)printf("+\n");
				else printf("+---");
			}
		} 
	}
	printf("\n");
	
}

void red () {
  printf("\033[1;31m");
}

void yellow() {
  printf("\033[1;33m");
}

void blue(){
printf("\033[0;34m");
}

void magenta(){
printf("\033[0;35m");
}

void reset () {
  printf("\033[0m");
}
#define TEST_ERROR if (errno) {fprintf(stderr,				\
				       "%s:%d: PID=%5d: Error %d (%s)\n", \
				       __FILE__,			\
				       __LINE__,			\
				       getpid(),			\
				       errno,				\
				       strerror(errno));}

/* Set a semaphore to a user defined value */
int sem_set_val(int sem_id, int sem_num, int sem_val) {

	return semctl(sem_id, sem_num, SETVAL, sem_val);
}

/* Try to access the resource */
int sem_reserve(int sem_id, int sem_num) {
	struct sembuf sops;
	
	sops.sem_num = sem_num;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	return semop(sem_id, &sops, 1);
}

/* Release the resource */
int sem_release(int sem_id, int sem_num) {
	struct sembuf sops;
  
	sops.sem_num = sem_num;
	sops.sem_op = 1;
	sops.sem_flg = 0;
	
	return semop(sem_id, &sops, 1);
}

/* Print all semaphore values to a string */
int sem_getall(char * my_string, int sem_id) {
	union semun arg;   /* man semctl per vedere def della union  */ 
	unsigned short * sem_vals, i;
	unsigned long num_sem;
	char cur_str[10];
	
	/* Get the number of semaphores */
	semctl(sem_id, 0, IPC_STAT, arg.buf);
	TEST_ERROR;
	num_sem = arg.buf->sem_nsems;
	
	/* Get the values of all semaphores */
	sem_vals = malloc(sizeof(*sem_vals)*num_sem);
	arg.array = sem_vals;
	semctl(sem_id, 0, GETALL, arg);
	
	/* Initialize the string. MUST be allocated by the caller */
	my_string[0] = 0;
	for (i=0; i<num_sem; i++) {
		sprintf(cur_str, "%d ", sem_vals[i]);
		strcat(my_string, cur_str);
	}
}

int aspetta_zero(int sem_id, int sem_num) {
    struct sembuf sops;
  
  sops.sem_num = sem_num;
  sops.sem_op = 0;
  sops.sem_flg = 0;
  
  return semop(sem_id, &sops, 1);   
}
int casuale(int a,int b)  
{  
    return rand() % (a - b + 1) + b;
}  

int val_check(int val, int *pos){
    int i;
    int ver = 0;
    size_t n = sizeof(pos) / sizeof(pos[0]);
    for(i; i<n; i++)if(pos[i] == val)ver = 1;
    return ver;
}

int initSemAvaiable(int semId, int semNum){  /*Inizializzo semaforo a 1 (disponibile) */
	union semun arg;
	arg.val = 1;
	return semctl(semId, semNum, SETVAL, arg);
}

int initSemInUse(int semId, int semNum){	/*Inizializzo semaforo a 0 (in uso)*/
	union semun arg;
	arg.val = 0;
	return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum){
	struct sembuf sops;
	sops.sem_num = semNum;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	
	return semop(semId, &sops, 1);
}

int releaseSem(int semId, int semNum){
	struct sembuf sops;
	sops.sem_num = semNum;
	sops.sem_op = 1;
	sops.sem_flg = 0;
	
	return semop(semId, &sops, 1);
	}