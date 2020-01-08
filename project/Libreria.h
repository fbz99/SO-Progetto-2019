#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>

struct stato_player{
	int pid;
	char giocatore;
};

struct stato_pedina{
	int pid;
	char padre;
	int mosse;	
};