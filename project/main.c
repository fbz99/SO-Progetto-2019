#include "Libreria.h"

int main()
{
    int i, size = SO_ALTEZZA * SO_BASE;
    int *ptr;
    int flag_n = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int tot = SO_ROUND_SCORE;
    int max_rand,cont = 0;
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band, j = 0, rand_pos, pos;
    int tmp = flag;
    int status;
    int *old_pos;
    pid_t wpid;
    char *args[3];
    char *matrice;
    char stringa[4],stringa1[4];
    int sem_id_mat, sem_id_mutex, sem_id_main;

    int mat_id = shmget(key, sizeof(int) * (SO_BASE * SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);
   
   

    /*for (pos = 0; pos <= size; pos++) /*SETTAGGIO MATRICE*/
        /*matrice[pos] = '0';*/

    /*Creazione semafori sulla matrice*/
    sem_id_mat = semget(key2, size, IPC_CREAT | 0666);
    for (i = 0; i < size; i++)
    {
        initSemAvaiable(sem_id_mat, i);
    }

    /*Creazione array di semafori per attesa dei giocatori (tutti a 0 tranne il primo)*/
    sem_id_mutex = semget(key3, 1, IPC_CREAT | 0666);
    initSemAvaiable(sem_id_mutex, 0);

    sem_id_main = semget(key1, SO_NUM_G, IPC_CREAT | 0666);
    for (i = 0; i < SO_NUM_G; i++)
    {
        initSemInUse(sem_id_main, i);
        printf("SEMCTL %d\n",semctl(sem_id_main,i,GETVAL));
    }

    args[2] = NULL;
    ptr = malloc(sizeof(int) * SO_NUM_G);

    /*Creazione Giocatori*/
    for (i = 0; i < SO_NUM_G; i++)
    {
        switch (ptr[i] = fork())
        {
        case -1:
            printf("Error\n");
            exit(0);
            break;
        case 0: /*Processo figlio*/
            sprintf(stringa, "%d", i + 1);
            args[0] = stringa;
            sprintf(stringa1,"%d",cont);
            args[1] = stringa1;
            execve("./player", args, NULL);
            break;

            default: 
                /* aspetta_zero(sem_id_main,i);*/
                /*printf("CONT: %d\n",cont);*/
                cont++;
                break;
        }
    }
    for(i=0;i<SO_NUM_G;i++) reserveSem(sem_id_main,i);
    /*while ((wpid = wait(&status)) > 0){ */
    /*waitPlayers(sem_id_main);*/

    old_pos = malloc(sizeof(int) * flag);
    srand(time(NULL));
    for (i = 0; i < flag; i++)
    {

        rand_pos = casuale(size, 0);
        old_pos[i] = rand_pos;
        if ((val_check(rand_pos, old_pos) == 1) && pos_check(rand_pos, matrice) == 1)
            rand_pos = casuale(size, 0);

        max_rand = tot - (tmp - 1);
        if (tmp != 1)
        {
            band = casuale(max_rand, 1);
        }
        else
            band = tot;
        tot = tot - band;

        if (band != 0)
        {
            reserveSem(sem_id_mat, rand_pos + 1);
            matrice[rand_pos] = band + 48;
        }
        tmp--;
    }
    /*stampa_scacchiera();*/
    printf("MATRICE\n");
   /* stampaArray(matrice);*/
    
    while (wait(NULL) != -1);
    shmctl(mat_id,IPC_RMID,NULL); /*Rimozione memoria condivisa*/
    for(i=0;i<SO_NUM_G;i++) semctl(sem_id_main,i,IPC_RMID);
}
