#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#define SO_NUM_G 2
#define SO_NUM_P 10
#define SO_MAX_TIME 3
#define SO_BASE 60
#define SO_ALTEZZA 20
#define SO_FLAG_MIN 5
#define SO_FLAG_MAX 5
#define SO_ROUND_SCORE 10
#define SO_N_MOVES 20
#define SO_MIN_HOLD_NSEC 10000000
#define key 12345
#define key0 44245 /*semidzero*/
#define key1 55245
#define key2 22345
#define key3 33245
#define key4 66245 /*semidped*/
#define MSG_LEN 120
#define key_coda 55245
#define MSGTYPE_RM 7

/*SEMAFORO IN USO --> 0
  SEMAFORO DISPONIBILE --> 1*/

void stampa_scacchiera();
void red();
void yellow();
void blue();
void magenta();
void reset();

struct stato_player{
	int pid;
	char giocatore;
};

struct stato_pedina{
	int pid;
	char padre;
	int mosse;
	int pos;	
};

struct msgbuf {
	long mtype;             /* Tipo de messaggio */
	char mtext[MSG_LEN];	/*Il messaggio verrà inserito qui*/
};

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
				    (Linux-specific) */
};


/*
 * Set a semaphore to a user defined value
 * INPUT:
 * - sem_id: the ID of the semaphore IPC object
 * - sem_num: the position of the semaphore in the array
 * - sem_val: the initialization value of the semaphore
 * RESULT:
 * - the selected semaphore is initialized to the given value
 * - the returned value is the same as the invoked semctl
 */
int sem_set_val(int sem_id, int sem_num, int sem_val);

/*
 * Try to access the resource
 * INPUT:
 * - sem_id: the ID of the semaphore IPC object
 * - sem_num: the position of the semaphore in the array
 * RESULT
 * - if the resource is available (semaphore value > 0), the semaphore
 *   is decremented by one
 * - if the resource is not available (semaphore value == 0), the
 *   process is blocked until the resource becomes available again
 * - the returned value is the same as the invoked semop
 */
int sem_reserve(int sem_id, int sem_num);

/*
 * Release the resource
 * INPUT:
 * - sem_id: the ID of the semaphore IPC object
 * - sem_num: the position of the semaphore in the array
 * RESULT:
 * - the semaphore value is incremented by one. This may unblock some
 *   process
 * - the returned value is the same as the invoked semop
 */
int sem_release(int sem_id, int sem_num);

/*
 * Print all semaphore values to a string. my_string MUST be
 * previously allocated
 */
int sem_getall(char * my_string, int sem_id);

int aspetta_zero(int sem_id, int sem_num);

int aspetta_uno(int sem_id, int sem_num);

int casuale(int a,int b);

int val_check(int val, int *pos);
 
int initSemAvaiable(int semId, int semNum);

int initSemInUse(int semId, int semNum);

int reserveSem(int semId, int semNum);

int releaseSem(int semId, int semNum);

int pos_check(int pos,char * matrix);

void copiaArray(char a[],char b[]);

char* itoa(int i, char b[]);

void waitPlayers(int sem_array);

void stampaArray();