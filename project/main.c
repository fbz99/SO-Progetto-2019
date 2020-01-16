#include "Libreria.h"

int casuale();
void stampa_scacchiera();

int main(){
    int i, size = SO_ALTEZZA*SO_BASE;
    int *ptr;
    int flag_n = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN; 
    key_t key = 12345;
    int tot = SO_ROUND_SCORE;
    int max_rand;                                           
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band, j=0, rand_pos, pos;
    int tmp = flag;
    char * args [2]; 
    char *matrice;     
    char stringa[4];       
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT |0666);
    matrice = shmat(mat_id, NULL, 0);
    printf("------------------ MAIN ----------------\n");

       	for (pos = 0; pos < size; pos++) /*SETTAGGIO MATRICE*/
		matrice[pos] = 0;
        stampa_scacchiera();
        
    printf("Master process: %d\n",getpid());


    args[1]=NULL;
    ptr = malloc(sizeof(int)*SO_NUM_G);
    /*Creazione Giocatori*/
    for(i=0 ;i<SO_NUM_G;i++){ 
       switch(ptr[i] = fork()){
           case -1: {
           printf("Error\n");
           exit(0);
           }

           case 0: /*Processo figlio*/
            sprintf(stringa, "%d", i+1);
            args[0]= stringa;
            printf("PID Giocatore : %d\n", getpid());
            execve("./player", args, NULL);
            

            /*default: printf("default fork\n");
            break;*/
       } 
       
    }

    execve("./bandierina",NULL,NULL);


    while (wait(NULL) != -1);
}

int casuale(int a,int b)  
{  
    return rand() % (b - a + 1) + a;
}  


void stampa_scacchiera(){
	int i,j;
    char *matrice;                                          
    key_t key = 12345;
    int mat_id = shmget (key, sizeof(int)*(SO_BASE*SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);

		
		for(i=0; i<SO_BASE;i++)printf(" __");
		printf("\n");
		for(i=0;i<SO_ALTEZZA*SO_BASE;i++){
			if(i%SO_BASE == 0) printf("\n");
            printf("|%d - %d",i, matrice[i]);}
	    printf("\n");
	}

