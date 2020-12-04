#include "Libreria.h"

void alarm_handle(int signal);
void sigint_handle(int signal);

int *ptr, *posPed, rand_pos = 0, *mossePed, *band;
int msg_ped, msg_master;
int varConfig_id;
int SO_NUM_P;

int main(int argc, const char *args[]){
    int SO_NUM_G, SO_MAX_TIME, SO_BASE, SO_ALTEZZA, SO_FLAG_MIN, SO_FLAG_MAX, SO_ROUND_SCORE, SO_N_MOVES, SO_MIN_HOLD_NSEC;
    int mat_id;
    int *assegnate;
    struct msg_pedina m_pedina;
    struct msg_master m_master;
    struct variabiliCondivise *varCond;
    struct stato_player giocatore;
    int i, j, dist, count, x, y, k;

    char *args1[1], *matrice;
    int sem_id_mat, sem_id_mutex, sem_id_zero, sem_round;
    
    struct sigaction sb;
	bzero(&sb,sizeof(sb));
	sb.sa_handler = sigint_handle;
	sigaction(SIGINT,&sb,NULL);

    /*Gestione delle variabili condivise*/
     setvbuf(stdout, NULL, _IONBF, 0); /* Rimuove Buffer */
    varConfig_id = shmget(key6, sizeof(int)*10, IPC_CREAT | 0600);
    varCond = shmat(varConfig_id, NULL, 0);
    SO_NUM_G = varCond->SO_NUM_G;
    SO_NUM_P = varCond->SO_NUM_P;
    SO_MAX_TIME = varCond->SO_MAX_TIME;
    SO_BASE = varCond->SO_BASE;
    SO_ALTEZZA = varCond->SO_ALTEZZA;
    SO_FLAG_MIN = varCond->SO_FLAG_MIN;
    SO_FLAG_MAX = varCond->SO_FLAG_MAX;
    SO_ROUND_SCORE = varCond->SO_ROUND_SCORE;
    SO_N_MOVES = varCond->SO_N_MOVES;
    SO_MIN_HOLD_NSEC = varCond->SO_MIN_HOLD_NSEC;

    mat_id = shmget(key, sizeof(int) * (SO_BASE * SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);
    sem_id_mat = semget(key2, (SO_BASE*SO_ALTEZZA), IPC_CREAT | 0666);

    posPed = malloc(sizeof(int) * SO_NUM_P); /*Tutte le posizioni delle pedine*/
    band = malloc(sizeof(int));
    
    /*Definizione struct giocatore*/
    giocatore.pid = getpid();
    giocatore.giocatore = (atoi(args[0])) + 64;

    /*Creazione coda di messaggi per master e pedine*/
    msg_ped = msgget(getpid(), IPC_CREAT | 0666);
    msg_master = msgget(key7, IPC_CREAT | 0666);

    /*Semafori*/
    sem_id_mutex = semget(key3,SO_NUM_G, IPC_CREAT | 0666);
    sem_id_zero = semget(key0, 3, IPC_CREAT | 0666);
    
    /*Creazione delle pedine --> Fork*/
    args1[0]=NULL;
    ptr = malloc(sizeof(int) * SO_NUM_P);
    for (i = 0; i < SO_NUM_P; i++){   
        switch (ptr[i] = fork()){
        case -1:
            fprintf(stderr,"Le pedine non sono state create\n");
            exit(EXIT_FAILURE);
            break;
        case 0: /*Processo figlio*/
            if(execve("./Pedina", args1, NULL) == -1){
                fprintf(stderr, "ERRORE!\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    for (i = 0; i < SO_NUM_P; i++){
        srand(ptr[i]);
        rand_pos = casuale(SO_BASE * SO_ALTEZZA, 1);
        posPed[i] = rand_pos;
        sem_reserve(sem_id_mutex, (giocatore.giocatore-65));
        
        while(val_check(rand_pos,posPed)){
            rand_pos = casuale(SO_BASE * SO_ALTEZZA, 1);
        }
        posPed[i] = rand_pos;
        m_pedina.pos  = rand_pos;
        m_pedina.type = ptr[i];
        m_pedina.padre = giocatore.giocatore;
        m_pedina.punteggio = 0;
        m_pedina.mosse = SO_N_MOVES;
        msgsnd(msg_ped,&m_pedina, (sizeof(int)*5), 0);

        /*Gestione semafori*/
        sem_reserve(sem_id_zero, 1); /*Per attendere che si piazzino le pedine*/
        sem_release(sem_id_mutex,(giocatore.giocatore-65+1)%SO_NUM_G); /*Sblocca il giocatore successivo*/
    }
    
    mossePed = malloc(sizeof(int) * SO_NUM_P);
    for(i = 0; i < SO_NUM_P; i++)mossePed[i] = SO_N_MOVES;
    sem_round = semget(key5, 2, 0666 | IPC_CREAT);
    sem_set_val(sem_round, 1, 0); 
    
    /*Viene sbloccato il main*/
    sem_reserve(sem_id_zero,0);

    while(1){
        sem_reserve(sem_round, 0);
        sem_set_val(sem_id_zero, 3, SO_NUM_G);
        count = 0;
        j = 0;
        for(i = 0; i < SO_BASE*SO_ALTEZZA; i++){
            if(matrice[i] > 0 && matrice[i] < 65){
                count++;
                band = realloc(band, sizeof(int)*count); /*Ridimensionamento dell'array */
                band[j++] = i;
            }
        }
        assegnate = malloc(sizeof(int) * SO_NUM_P);
        for(i =0; i<SO_NUM_P;i++){
            assegnate[i] = -1;
        }
        k = -1;
        for(j = 0; j < count; j++){
            dist = SO_N_MOVES+1;
            for(i = 0 ; i < SO_NUM_P; i++){
                /*Manhattan distance per cammino minimo*/
                if((assegnate[i] == -1) && (abs(calcR(posPed[i], SO_BASE) - calcR(band[j], SO_BASE)) + abs(calcC(calcR(posPed[i], SO_BASE), posPed[i], SO_BASE) - calcC(calcR(band[j], SO_BASE),band[j], SO_BASE)) <=mossePed[i]) 
                && (abs(calcR(posPed[i], SO_BASE) - calcR(band[j], SO_BASE)) + abs(calcC(calcR(posPed[i], SO_BASE), posPed[i], SO_BASE) - calcC(calcR(band[j], SO_BASE),band[j], SO_BASE)) < dist)){
                    dist = abs(calcR(posPed[i], SO_BASE) - calcR(band[j], SO_BASE)) + abs(calcC(calcR(posPed[i], SO_BASE), posPed[i], SO_BASE) - calcC(calcR(band[j], SO_BASE),band[j], SO_BASE));
                    k = i;
                }
            }
            if(k != -1){
                assegnate[k] = band[j];
                k = -1;
            }
        }
        for(i=0;i<SO_NUM_P;i++){
            if(assegnate[i]==-1){
                m_pedina.obbPos = posPed[i];
            }
            else{
                m_pedina.obbPos = assegnate[i];
            }
            m_pedina.type = ptr[i];
            msgsnd(msg_ped,&m_pedina,((sizeof(int)*5)),0);
        }
        free(assegnate);
        sem_reserve(sem_id_zero, 0);
        aspetta_zero(sem_id_zero, 2);
        sem_reserve(sem_id_zero, 3);
        for(i=0; i < SO_NUM_P; i++){
            msgrcv(msg_ped, &m_pedina,sizeof(int)*5, ptr[i], 0);
            m_master.type = 1;
            m_master.player = giocatore.giocatore;
            m_master.punteggio = m_pedina.punteggio;
            m_master.mosseRimaste = m_pedina.mosse;
            msgsnd(msg_master, &m_master, sizeof(int) * 3, 0);
            mossePed[i] = m_pedina.mosse;
            posPed[i] = m_pedina.pos;
        }
    }
}

void sigint_handle(int signal){
	int i;
	msgctl(msg_ped,IPC_RMID,NULL);
	for (i = 0; i < SO_NUM_P; i++){
        kill(ptr[i], SIGINT);
    }
	free(ptr);
	free(posPed);
	free(band);
	free(mossePed);
	while (wait(NULL) != -1);	
	exit(EXIT_SUCCESS);	
}
