#include "Libreria.h"


int main(){
    int i, size = SO_ALTEZZA*SO_BASE;
    int *ptr;
    int flag_n = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN; 
    key_t key = 12345, key2 = 22345, key3 = 33245, key0 = 44245;
    int tot = SO_ROUND_SCORE;
    int max_rand;                                           
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band, j=0, rand_pos, pos;
    int tmp = flag;
    int status; 
    int *old_pos;
    pid_t wpid;
    char * args [2]; 
    char *matrice;     
    char stringa[4];  
    int sem_id_mat, sem_id_mutex, sem_id_zero;     
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT |0666);
    matrice = shmat(mat_id, NULL, 0);
   
       	for (pos = 0; pos <= size; pos++) /*SETTAGGIO MATRICE*/
		matrice[pos] = '0';
		/*Creazione semafori sulla matrice*/
		sem_id_mat= semget (key2, size, IPC_CREAT | 0666);
		for(i=0;i<size;i++){
			sem_set_val(sem_id_mat,i,1);
		}

		/*Creazione mutex per accesso alla matrice*/
		sem_id_mutex = semget (key3, SO_NUM_G, IPC_CREAT | 0666);
		for(i=0; i<SO_NUM_G;i++){
			sem_set_val(sem_id_mutex,i,0);
		}

		sem_id_zero = semget(key0,4, IPC_CREAT | 0666);
    
    args[1]=NULL;
    ptr = malloc(sizeof(int)*SO_NUM_G);
    /*Creazione Giocatori*/
    for(i=0 ;i<SO_NUM_G;i++){ 
       switch(ptr[i] = fork()){
           case -1: 
           printf("Error\n");
           exit(0);
           

           case 0: /*Processo figlio*/
            sprintf(stringa, "%d", i+1);
            args[0]= stringa;
            /*printf("PID Giocatore : %d\n", getpid());
            */execve("./player", args, NULL);
            

            /*default: printf("default fork\n");
            break;*/
       } 
       
    }
    sem_set_val(sem_id_zero, 0 ,SO_NUM_G);
    sem_set_val(sem_id_zero,2,1);
    /*aspetta_zero(sem_id_zero,0);*/
    while ((wpid = wait(&status)) > 0); { 
    old_pos = malloc(sizeof(int)*flag);

    
    
    for(i=0;i<flag;i++){
        
        rand_pos = casuale(size, 0);
        old_pos[i]=rand_pos;
        if(val_check(rand_pos, old_pos) == 1)rand_pos = casuale(size,0);
        
    	max_rand = tot-(tmp-1);
    	if(tmp != 1){
            band = casuale(max_rand,1);
        }
    	else band = tot;
        tot = tot - band;
        
        if(band!=0)
        matrice[rand_pos] = band+48;
        
    	tmp--;
        
    }

    
    stampa_scacchiera();
    }
    /*shmctl(mat_id, IPC_RMID, NULL); /*RIMOZIONE MEMORIA CONDIVISA*/
    while (wait(NULL) != -1);
}



