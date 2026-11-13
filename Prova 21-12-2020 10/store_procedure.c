#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "header.h"
#include "store.h"

extern int ds_queue_request, ds_queue_response;

void Wait_Sem(int ds_sem, int id_sem) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = id_sem;
    buf.sem_op = -1;
    semop(ds_sem,&buf,1);
}

void Signal_Sem(int ds_sem, int id_sem) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = id_sem;
    buf.sem_op = 1;
    semop(ds_sem,&buf,1);
}

void init_magazzino(Magazzino *magazzino){
    // TODO inizializzazione dei semafori , delle variabili necessarie alla sincronizzazione
    // e delle scorte di magazzino iniziali

    for(int j=0; j<MAX_PRODOTTI; j++){
    magazzino->scorta[j]= SCORTA_INIZIALE;
    magazzino->num_lettori[j]= 0;
    }

    magazzino->id_sem= semget(IPC_PRIVATE, 4, IPC_CREAT|0664);

    semctl(magazzino->id_sem, MUTEX_L, SETVAL, 1);
    semctl(magazzino->id_sem, 0, SETVAL, 1);
    semctl(magazzino->id_sem, 1, SETVAL, 1);
    semctl(magazzino->id_sem, 2, SETVAL, 1);

    magazzino->id_coda_req= msgget(ftok("./", 'f'), 0);
    magazzino->id_coda_resp= msgget(ftok("./", 'g'), 0);
}

void destroy_magazzino(Magazzino *magazzino){
    // TODO rimozione dei semafori
    semctl(magazzino->id_sem, 0, IPC_RMID); 
}

void lettore(Magazzino *magazzino) {
    int ret;
    Msg_Req m_req;
    Msg_Resp m_resp;
    int k;

    for (k=0; k<4; k++) {
        // TODO aggiungere codice per la ricezione comando
        int result= msgrcv(magazzino->id_coda_req, (void *)&m_req, sizeof(Msg_Req)-sizeof(long), READ, 0);

        if(result<0){perror("[LETTORE]: Errore nella ricezione di un messaggio dalla coda richieste\n"); exit(1);}

        int id_prodotto= m_req.id_prodotto;
        // TODO Aggiungere codice per la lettura:
        // InizioLettura
        InizioLettura(magazzino, id_prodotto);
        printf("[LETTORE %d]: Inizio Lettura...\n", getpid());
        // Lettura del livello di scorta del prodotto il cui id è indicato nella richiesta
        int livello_scorta= magazzino->scorta[id_prodotto];
        printf("[LETTORE %d]: Livello scorta: %d\n", getpid(), livello_scorta);
        // FineLettura
        printf("[LETTORE %d]: Fine Lettura...\n", getpid());
        FineLettura(magazzino, id_prodotto);

        // TODO Costruire il messaggio di risposta m_resp
        m_resp.id_prodotto= id_prodotto;
        m_resp.pid= m_req.mittente;
        printf("[LETTORE %d]: Valore mittente: %ld\n", getpid(), m_resp.pid);
        m_resp.scorta= livello_scorta;
    

        // TODO aggiungere codice per l'invio del messaggio di risposta
        m_resp.id_prodotto= id_prodotto;
        m_resp.pid= m_req.mittente;
        m_resp.scorta= livello_scorta;

        printf("[lettore %d] Ricevuta richiesta di lettura per prodotto %d con livello scorte %d\n", getpid(), m_resp.id_prodotto,m_resp.scorta);

        int result2= msgsnd(magazzino->id_coda_resp, (void *)&m_resp, sizeof(Msg_Resp)-sizeof(long), 0);

        if(result2<0){perror("[LETTORE]: Errore nell'invio di un messaggio alla coda risposte\n"); exit(1);}
    }
}

void scrittore(Magazzino *magazzino){
    int ret;
    Msg_Req m_req;
    int k;

    for (k = 0; k < 2; k++) {
        // TODO agiungere codice per la ricezione del comando
        int result= msgrcv(magazzino->id_coda_req, (void *)&m_req, sizeof(Msg_Req)-sizeof(long), WRITE, 0);

        if(result<0){perror("[SCRITTORE]: Errore nella ricezione di un messaggio dalla coda richieste\n"); exit(1);}

        // TODO aggiungere codice per la scrittura
        int id_prodotto= m_req.id_prodotto;
        int valore= m_req.value;
        // InizioScrittura
        InizioScrittura(magazzino, id_prodotto);
        printf("[SCRITTORE %d]: Inizio Scrittura...\n", getpid());
        // Decremento del livello di scorta del prodotto indicato nella richiesta della quantità indicata nella richiesta
        magazzino->scorta[id_prodotto]= magazzino->scorta[id_prodotto]- valore;
        // FineScrittura
        printf("[SCRITTORE %d]: Fine Scrittura...\n", getpid());
        FineScrittura(magazzino, id_prodotto);

        printf("[scrittore %d] Ricevuta richiesta di decremento di %d per prodotto %d\n",getpid(), m_req.value, m_req.id_prodotto);
    }
}

void InizioLettura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di inizio lettura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
    Wait_Sem(magazzino->id_sem, MUTEX_L);

    magazzino->num_lettori[id_prodotto]++;

    if(magazzino->num_lettori[id_prodotto]==1){
    printf("[INIZIO_LETTURA]: Sono il primo lettore, fermo gli scrittori\n");
    Wait_Sem(magazzino->id_sem, id_prodotto);
    }

    Signal_Sem(magazzino->id_sem, MUTEX_L);
}

void FineLettura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di fine lettura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori

    Wait_Sem(magazzino->id_sem, MUTEX_L);

    magazzino->num_lettori[id_prodotto]--;

    if(magazzino->num_lettori[id_prodotto]==0){
        printf("[FINE_LETTURA]: Sono l'ultimo lettore, sveglio uno scrittore\n");
        Signal_Sem(magazzino->id_sem, id_prodotto);
    }

    Signal_Sem(magazzino->id_sem, MUTEX_L);
}

void InizioScrittura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di inizio scrittura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
    Wait_Sem(magazzino->id_sem, id_prodotto);
}
void FineScrittura(Magazzino *magazzino, int id_prodotto){
    //TODO aggiungere il codice di fine scrittura, utilizzando i semafori
    // secondo lo schema lettori-scrittori con starvation degli scrittori
    Signal_Sem(magazzino->id_sem, id_prodotto);
}
