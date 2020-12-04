#include "Libreria.h"

int main(){
    int SO_NUM_G, SO_NUM_P, SO_MAX_TIME, SO_BASE, SO_ALTEZZA, SO_FLAG_MIN, SO_FLAG_MAX, SO_ROUND_SCORE, SO_MIN_HOLD_NSEC;
    int varConfig_id;
    struct variabiliCondivise *varCond;
    int mat_id;
    char *matrice;
    int sem_id_mat,sem_id_zero, sem_round;
    struct msg_pedina m_pedina;
    struct pedina pedina;
    int msg_ped;
    int mover, movec, obbPos, obbR, obbC, pos, bandierina, rit, startingPos;
    struct timespec ts;

    /*Configurazione Variabili Condivise*/
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
    SO_MIN_HOLD_NSEC = varCond->SO_MIN_HOLD_NSEC;

    ts.tv_sec = 0;
    ts.tv_nsec = SO_MIN_HOLD_NSEC;
    mat_id = shmget(key, sizeof(int) * (SO_BASE * SO_ALTEZZA), IPC_CREAT | 0666);
    matrice = shmat(mat_id, NULL, 0);
    sem_id_mat = semget(key2, SO_BASE * SO_ALTEZZA, IPC_CREAT | 0666);

    
    /*Messaggio dal giocatore*/
    msg_ped = msgget(getppid(), IPC_CREAT | 0666);
    msgrcv(msg_ped, &m_pedina, (sizeof(int)*5), getpid(), 0);
    pedina.pid = getpid();
    pedina.pos = m_pedina.pos;
    pedina.mosse = m_pedina.mosse;
    pedina.padre = m_pedina.padre;
    startingPos = pedina.pos;
    
    /*Posiziono la pedina sulla scacchiera*/
    matrice[pedina.pos] = pedina.padre;

    sem_id_zero = semget(key0,3, IPC_CREAT | 0666);   
	sem_release(sem_id_zero,1);

    sem_round = semget(key5, 2, 0666 | IPC_CREAT);
    while(1){
        /*Ricevo la strategia dal giocatore*/
        msgrcv(msg_ped,&m_pedina,sizeof(int)*5,getpid(),0);
        obbPos = m_pedina.obbPos;
        obbR = calcR(obbPos, SO_BASE);
        obbC = calcC(obbR, obbPos, SO_BASE);
        /* Posso muovermi */
        aspetta_zero(sem_id_zero,3);

        /*Inizio ricerca bandierine*/
        bandierina = 0;  
        pos = pedina.pos;
        mover = 1;
        movec = 1;
        while ((calcR(pos, SO_BASE) != obbR || calcC(calcR(pos, SO_BASE), pos, SO_BASE) != obbC)){  
            /* Righe */
            if (pedina.mosse == 0) break;
            if (mover == 1){
                if (calcR(pos, SO_BASE) < obbR){
                    rit = sem_reserve_wait_time(sem_id_mat, pos + SO_BASE);
                    if (rit == -1 && errno == EAGAIN){
                        mover = 0;
                        continue;
                    }
                    else {
                        movec = 1;
                        sem_release(sem_id_mat,pos);
                        matrice[pos] = 0;
                        nanosleep(&ts,NULL);
                        pos = pos + SO_BASE;
                        if((matrice[pos]) > 0 && (matrice[pos]) < 65){
                            bandierina = matrice[pos];
                        } 
                        matrice[pos] = pedina.padre;
                        pedina.pos = pos;
                        pedina.mosse--;
                        if (bandierina != 0){
                            m_pedina.type = getpid();
                            m_pedina.mosse = pedina.mosse;
                            m_pedina.punteggio = bandierina;
                            m_pedina.pos = pos;
                                msgsnd(msg_ped,&m_pedina,sizeof(int)*5,0);
                            break;   
                        }
                    }
                }
                else {
                    if (calcR(pos, SO_BASE) > obbR){
                        rit = sem_reserve_wait_time(sem_id_mat,pos - SO_BASE);
                        if (rit == -1 && errno == EAGAIN){
                            mover = 0;
                            continue;
                        }
                        else {
                            movec = 1;
                            sem_release(sem_id_mat,pos);
                            matrice[pos] = 0;
                            nanosleep(&ts,NULL);
                            pos = pos - SO_BASE;
                            if((matrice[pos]) > 0 && (matrice[pos]) < 65){
                                bandierina = matrice[pos];
                            }
                            matrice[pos] = pedina.padre;
                            pedina.pos = pos;
                            pedina.mosse--;
                            if (bandierina != 0){
                                m_pedina.type = getpid();
                                m_pedina.mosse = pedina.mosse;
                                m_pedina.punteggio = bandierina;
                                m_pedina.pos = pos;
                                msgsnd(msg_ped,&m_pedina,sizeof(int)*5,0);
                                break;   
                            }
                        }
                    }
                    else {
                        if (movec == 0) break; /*Riga giusta + Non posso muovermi = break*/
                    }
                }
            }
            if (pedina.mosse == 0) break;
            
            /* Colonne */
            if (movec == 1){
                if (calcC(calcR(pos, SO_BASE), pos, SO_BASE) < obbC){
                    rit = sem_reserve_wait_time(sem_id_mat,pos + 1);
                    if (rit == -1 && errno == EAGAIN){
                        movec = 0;
                        continue;
                    }
                    else {
                        mover = 1;
                        sem_release(sem_id_mat,pos);
                        matrice[pos] = 0;
                        nanosleep(&ts,NULL);
                        pos++;
                        if((matrice[pos]) > 0 && (matrice[pos]) < 65){
                            bandierina = matrice[pos];
                        }
                        matrice[pos] = pedina.padre;
                        pedina.pos = pos;
                        pedina.mosse--;
                        if (bandierina != 0){
                            m_pedina.type = getpid();
                            m_pedina.mosse = pedina.mosse;
                            m_pedina.punteggio = bandierina;
                            m_pedina.pos = pos;
                            msgsnd(msg_ped,&m_pedina,sizeof(int)*5,0);
                            break;   
                        }
                    }
                }
                else {
                    if (calcC(calcR(pos, SO_BASE), pos, SO_BASE) > obbC){
                        rit = sem_reserve_wait_time(sem_id_mat,pos - 1);
                        if (rit == -1 && errno == EAGAIN){
                            movec = 0;
                            continue;
                        }else {
                            mover = 1;
                            sem_release(sem_id_mat,pos);
                            matrice[pos] = 0;
                            nanosleep(&ts,NULL);
                            pos--;
                            if ((matrice[pos] > 0) && (matrice[pos] < 65)){ 
                                bandierina = matrice[pos];
                            }
                            matrice[pos] = pedina.padre;
                            pedina.pos = pos;
                            pedina.mosse--;
                            if (bandierina != 0){
                                m_pedina.type = getpid();
                                m_pedina.mosse = pedina.mosse;
                                m_pedina.punteggio = bandierina;
                                m_pedina.pos = pos;
                                msgsnd(msg_ped,&m_pedina,sizeof(int)*5,0);
                                break;   
                            }
                        }
                    }
                    else {
                        if (mover == 0) break; /*Colonna giusta + Non posso muovermi = break*/
                    }
                }
            }
            if (mover == 0 && movec == 0) break;

        }
        if (bandierina == 0){
            m_pedina.type = getpid();
            m_pedina.mosse = pedina.mosse;
            m_pedina.punteggio = bandierina;
            m_pedina.pos = pos;
            msgsnd(msg_ped,&m_pedina,sizeof(int)*5,0);
        } 
        sem_reserve(sem_round,1);
    }
}