#include "Libreria.h"

void alarm_handle(int signal);
void sigint_handle(int signal);

char *matrice;
struct variabiliCondivise *varCond;
struct msg_master m_master;
int SO_NUM_G, SO_NUM_P, SO_MAX_TIME, SO_BASE, SO_ALTEZZA, SO_FLAG_MIN, SO_FLAG_MAX, SO_ROUND_SCORE, SO_N_MOVES, SO_MIN_HOLD_NSEC;
int varConfig_id, mat_id;
int nRound = 0, msg_master;
int *punteggioPl, *mossePl;
int *ptr;
int sem_id_mat, sem_id_mutex, sem_id_zero, sem_round;
double tempo;
time_t fine, inizio;

int main(){
    int i, size,tot, media, flag, totflag;
    int band, rand_pos, pos, tmp, hard = 0;
    struct sigaction sa;
	struct sigaction sc;
    pid_t wpid;
    char *args[2];
    char stringa[4];

    bzero(&sa,sizeof(sa));
	sa.sa_handler = alarm_handle;
	sigaction(SIGALRM,&sa,NULL);
	bzero(&sc,sizeof(sc));
	sc.sa_handler = sigint_handle;
	sigaction(SIGINT,&sc,NULL);
    
    /*Gestione delle variabili condivise*/
    setvbuf(stdout, NULL, _IONBF, 0); /* Rimuove Buffer */
    config_var_cond();
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

    size = SO_BASE*SO_ALTEZZA;
    if(SO_NUM_G > 2) hard = 1;

    /*Creazione array in memoria condivisa e inizializzazione*/
    mat_id = shmget(key, sizeof(int) * (SO_BASE * SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);
    for (pos = 0; pos < size; pos++)
        matrice[pos] = 0;

    /*Creazione punteggi/mosse rimanenti e inizializzazione*/
    punteggioPl = malloc(sizeof(int)*SO_NUM_G);
    mossePl = malloc(sizeof(int)*SO_NUM_G);
    for(i=0; i<SO_NUM_G;i++){
        punteggioPl[i] = 0;
        mossePl[i] = 0;                    
    }

    /*Creazione semafori per la matrice e inizializzazione*/
    sem_id_mat = semget(key2, size, IPC_CREAT | 0666);
    for (i = 0; i < size; i++){
        sem_set_val(sem_id_mat,i,1);
    }
    
    /*Creazione array di semafori per attesa dei giocatori*/
    sem_id_mutex = semget(key3, SO_NUM_G, IPC_CREAT | 0666);
    for(i = 0; i < SO_NUM_G; i++){
        sem_set_val(sem_id_mutex,i,0); /*Semafori settati a zero in attesa di essere liberati*/
    }

    sem_id_zero = semget(key0, 3, IPC_CREAT | 0666);
    
    args[1] = NULL;
    ptr = malloc(sizeof(int) * SO_NUM_G); /*Creazione della malloc per l'array contentente i giocatori*/

    /*Creazione Giocatori --> Fork*/
    msg_master = msgget(key7, IPC_CREAT | 0666);
    for (i = 0; i < SO_NUM_G; i++){
        switch (ptr[i] = fork()){
        case -1:
            printf("I giocatori non sono stati creati\n");
            exit(EXIT_FAILURE);
            break;
        case 0: /*Processo figlio*/
            sprintf(stringa, "%d", i + 1);
            args[0] = stringa;
            if(execve("./Player", args, NULL)==-1){
                fprintf(stderr, "ERRORE!\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    /*Semaforo per gestione dei round*/
    sem_round = semget(key5, 2, 0666 | IPC_CREAT);
    sem_set_val(sem_round, 0, 0);

    sem_set_val(sem_id_zero, 1, 0); /*Player attende piazzamento pedina*/
	sem_set_val(sem_id_mutex,0,1); /* Il primo giocatore può piazzare la pedina */
	sem_set_val(sem_id_zero, 0, SO_NUM_G); /* Attendo tutti i giocatori abbiano piazzato le pedine */
	aspetta_zero(sem_id_zero, 0);
    
    inizio = time(NULL);
    srand(time(NULL));
    while(1){
        tot = SO_ROUND_SCORE;
        totflag=rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1) + SO_FLAG_MIN;
        flag = totflag;
        tmp = flag;
        media = tot/totflag;
        band=0;
        nRound++;
        /*Generazione posizioni casuali delle bandierine*/
        while ((flag) > 0){	
            if(hard == 0){
			    if (flag == 1){
                    band = tot;
                }
			    else {
			    	media = tot/flag;
			    	band = rand()% media + 1;
                }
			}
            else if(hard == 1){
                if(flag == 1){
                    band = tot;
                }
                else{
			    	band = media;
                }
            }
			do{
				rand_pos =  rand()%((size+1)-0) + 0;
			}while (matrice[rand_pos] != 0 || rand_pos == (SO_ALTEZZA * SO_BASE));
			matrice[rand_pos] = band;
			tot = tot - band;
			flag--;
		}
        stampa_scacchiera(SO_BASE, SO_ALTEZZA);

        sem_set_val(sem_id_zero,2,1);   /*I giocatori attendono per iniziare il movimento*/
        sem_set_val(sem_id_zero,0,SO_NUM_G); /**/
        sem_set_val(sem_round,0,SO_NUM_G);  /*Sblocca nuovi round*/
        aspetta_zero(sem_id_zero, 0);   /*Attesa che i giocatori diano indicazioni alle pedine*/

        alarm(SO_MAX_TIME);  /*Tempo limite*/
        sem_reserve(sem_id_zero, 2);

        for(i=0; i < SO_NUM_G * SO_NUM_P; i++){
            msgrcv(msg_master, &m_master, sizeof(int)*3,1,0);
            mossePl[i] = 0;
            mossePl[m_master.player - 65] += m_master.mosseRimaste;
            punteggioPl[m_master.player - 65] += m_master.punteggio;
            if(m_master.punteggio > 0)tmp--;
            if(tmp == 0)alarm(0);
        }
        while(tmp > 0);

        stampa_scacchiera(SO_BASE, SO_ALTEZZA);
        magenta();
        for(i = 0;i < SO_NUM_G; i++){
            printf("PLAYER %c ---- | PUNTEGGIO %d | MOSSE RIMASTE %d|\n", 65+i, punteggioPl[i],mossePl[i]);
        }
        
        yellow();
        sem_set_val(sem_round, 1, SO_NUM_P * SO_NUM_G);
    }
}
void alarm_handle(int signal){
	int i, punti_totali_g;
	float mosse_tot;
	
	fine=time(NULL);
	tempo=((double)(fine-inizio));
    
    red();
	printf("\nPARTITA FINITA\n");
    reset();
	punti_totali_g = 0;
    for(i = 0; i< SO_NUM_G; i++){
        blue();
        printf("PLAYER %c ---- | PUNTEGGIO %d | MOSSE RIMASTE %d|\n", 65+i, punteggioPl[i],mossePl[i]);
        reset();
    }
	stampa_scacchiera(SO_BASE,SO_ALTEZZA);
	/* STAMPA RISULTATI */	
    red();
	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-RISULTATI+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    reset();
    yellow();
    printf("\n");
	printf("ROUND TOTALI: %d\n",nRound);
    reset();
	mosse_tot = SO_NUM_P*SO_N_MOVES;
    blue();
    printf("\n");
	printf("RAPPORTO mosse giocatori:\n");
    reset();
	for (i = 0; i < SO_NUM_G; i++)	
		printf("_ Player %c: %f\n",65+i,((SO_N_MOVES*SO_NUM_P)-mossePl[i])/mosse_tot);
	blue();
    printf("RAPPORTO Punti/Mosse\n");
    reset();
	for (i = 0; i < SO_NUM_G; i++)
		printf("_ Player %c: %f\n",65+i,punteggioPl[i]/(mosse_tot-mossePl[i]));
    blue();
    printf("INIZIO: %ld FINE: %ld TEMPO DI GIOCO: %f\n",inizio, fine, tempo);
    reset();
    red();
    printf("\n");
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n");
    reset();
    /*Rimozione memorie condivise e semafori*/ 
	shmctl(varConfig_id, IPC_RMID, NULL);
    shmdt(varCond);
    semctl(sem_id_mutex,0,IPC_RMID);
	semctl(sem_id_zero,0,IPC_RMID); /* 0 è ignorato*/
	semctl(sem_id_mat,0,IPC_RMID);
    semctl(sem_round,0,IPC_RMID);
	msgctl(msg_master,IPC_RMID,NULL);
    shmctl(mat_id, IPC_RMID, NULL);
	shmdt(matrice);
    /*kill di tutti i giocatori*/
	for (i = 0; i < SO_NUM_G; i++){
        kill(ptr[i], SIGINT);
    }
	while (wait(NULL) != -1);	
	free(punteggioPl);
	free(mossePl);
	free(ptr);
	exit(EXIT_SUCCESS);
}

void sigint_handle(int signal){
	int i;
	printf("\nPARTITA TERMINATA\n");
	shmctl(mat_id, IPC_RMID, NULL); 
	shmctl(varConfig_id, IPC_RMID, NULL);
	semctl(sem_round,0,IPC_RMID);
	semctl(sem_id_zero,0,IPC_RMID); /* 0 è ignorato*/
    semctl(sem_id_mat,0,IPC_RMID);
	semctl(sem_id_mutex,0,IPC_RMID);
	msgctl(msg_master,IPC_RMID,NULL);
	shmdt(matrice);
	shmdt(varCond);	
	while (wait(NULL) != -1);	
	exit(EXIT_SUCCESS);
}
