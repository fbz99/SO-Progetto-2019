#include "Libreria.h"

int main()
{
    int i, size = SO_ALTEZZA * SO_BASE;
    int *ptr;
    int flag_n = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int tot = SO_ROUND_SCORE;
    int max_rand;
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band, j = 0, rand_pos, pos;
    int tmp = flag;
    int status;
    int *old_pos;
    pid_t wpid;
    char *args[2];
    char *matrice;
    char stringa[4];
    int sem_id_mat, sem_id_mutex, sem_id_zero;
    int mat_id = shmget(key, sizeof(int) * (SO_BASE * SO_ALTEZZA), IPC_CREAT | 0666);

    matrice = shmat(mat_id, NULL, 0);
    /*printf("%d", mat_id);*/

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

    /*sem_id_zero = semget(key0,4, IPC_CREAT | 0666);*/
    args[1] = NULL;
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
            execve("./player", args, NULL);
            printf("PID Giocatore : %d\n", getpid());   /*Questa stampa non la fa probabilmente perchè il figlio muore dopo
                                                          l'execve, è giusto che faccia così?*/
            break;

            /*default: 
                printf("default fork\n");
                break;*/
        }
    }
    /*aspetta_zero(sem_id_zero,0);*/
    /*while ((wpid = wait(&status)) > 0){ */

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
    stampa_scacchiera();
    shmctl(mat_id,IPC_RMID,NULL); /*Rimozione memoria condivisa*/
    while (wait(NULL) != -1);
}
