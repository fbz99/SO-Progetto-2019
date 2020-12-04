#define _GNU_SOURCE /*Per gestione tempo*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <time.h>
#include <signal.h>

#define key 12345	/*Chiave memoria matrice*/
#define key0 44245 	/*semidzero*/
#define key1 75245	/*Coda di messaggi Pedina*/
#define key2 22345	/*Semafori matrice*/
#define key3 33245	/*Semafori mutua esclusione giocatori*/
#define key4 66245 	/*semidped*/
#define key5 76245 	/*Semafori round*/
#define key6 87542 	/*Variabili condivise*/
#define key7 97542	/*Coda di messaggi Master*/

struct stato_player{
	int pid;
	char giocatore;
};

struct msg_pedina{
	long type;
	char padre;
	int mosse;
	int punteggio;
	int pos;
	int obbPos;
};

struct msg_master{
	long type;
	int mosseRimaste;
	int punteggio;
	int player;
};

struct pedina{
	int pid;
	int pos;
	int mosse;
	int padre;
};

struct variabiliCondivise{
	int SO_NUM_G;
	int SO_NUM_P;
	int SO_MAX_TIME;
	int SO_BASE;
	int SO_ALTEZZA;
	int SO_FLAG_MIN;
	int SO_FLAG_MAX;
	int SO_ROUND_SCORE;
	int SO_N_MOVES;
	int SO_MIN_HOLD_NSEC;
};

/*Stampa della scacchiera e colori*/
void stampa_scacchiera(int base, int altezza);
void red();
void yellow();
void blue();
void magenta();
void reset();

/*Funzioni ausiliarie/semafori*/
int sem_set_val(int sem_id, int sem_num, int sem_val);
int sem_reserve(int sem_id, int sem_num);
int sem_release(int sem_id, int sem_num);
int aspetta_zero(int sem_id, int sem_num);
int casuale(int a,int b);
int val_check(int val, int *pos);
int sem_reserve_wait_time(int sem_id, int sem_num);
int calcR(int pos, int col);
int calcC(int r, int pos, int col);
void config_var_cond();
