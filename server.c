#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header_msg.h"
#include "header_sem.h"


void server(/* TBD: Completare il passaggio dei parametri */ lettscritt* ls, int id_sem) {

    for(int i=0; i<10; i++) {

        sleep(1);

        int valore = leggi(/* TBD */id_sem, ls);

        printf("[SERVER WORKER %d] Letto valore %d\n", getpid(), valore);

    }

}

int main() {


    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t chiave_coda_mess= ftok("./", 'f');

    int id_coda_mess= msgget(chiave_coda_mess, 0);

    if(id_coda_mess<0){
        printf("[SERVER]: Errore nella creazione della coda dei messaggi\n");
        exit(1);
    }



    int sem[NUM_BUFFER];
    int shm[NUM_BUFFER];
    lettscritt * ls[NUM_BUFFER];


    for(int i=0; i<NUM_BUFFER; i++) {

        /* TBD: Allocare il buffer su memoria condivisa */
        shm[i]= shmget(IPC_PRIVATE, sizeof(lettscritt), IPC_CREAT|0664);

        ls[i]= (lettscritt*)shmat(shm[i], NULL, 0);


        if(ls[i]==(void*)-1){printf("[SERVER]: Errore nell'attach della memoria condivisa n. %d\n",i+1); exit(1);}

        //ls[i] = /* TBD */(lettscritt*)shmat(shm[i], NULL, 0);
        //shm[i] = /* TBD */shmget(IPC_PRIVATE, sizeof(lettscritt), IPC_CREAT|0664);

        printf("[SERVER FRONTEND %d] Creato buffer %d (p=%p, id=%d)\n", getpid(), i, ls[i], shm[i]);

        ls[i]->buffer=0;
        ls[i]->num_lettori=0;
        ls[i]->num_scrittori=0;


        sem[i] = /* TBD */semget(IPC_PRIVATE, 2, IPC_CREAT|0664);

        printf("[SERVER FRONTEND %d] Creato vettore semafori %d (id=%d)\n", getpid(), i, sem[i]);

        semctl(sem[i], MUTEXL, SETVAL, 1);
        semctl(sem[i], SYNCH, SETVAL, 1);

    }


    for(int i=0; i<NUM_SERVER; i++) {

        /* TBD: 
            Creare un processo figlio (server worker),
            e fargli eseguire la funzione "server()".

            Passare al worker il riferimento
            a uno dei buffer e ai relativi semafori.

            Esempio: allo i-esimo processo server worker
            dovrà essere passato il semaforo "sem[i % NUM_BUFFER]".
        */
        pid_t pid= fork();
        if(pid==0){
            printf("[SERVER]: Creazione del processo figlio worker...\n");
            server(ls[i % NUM_BUFFER],sem[i % NUM_BUFFER]);
            exit(0);
        }else if(pid<0){
            perror("[SERVER]: Errore nella creazione del figlio Worker\n");
            exit(1);
        }

    }



    for(int i=0; i<NUM_RICHIESTE; i++) {


        /* TBD: Effettuare la ricezione del messaggio dal client (send sincrona) */
        MessaggioClient mess;

        int result= msgrcv(id_coda_mess, (void *)&mess, sizeof(MessaggioClient)-sizeof(long), REQUEST_TO_SEND, 0);
        if(result<0){perror("[SERVER]: Errore nella ricezione del messaggio RQST_TO_SND dal client\n"); exit(1); }

        MessaggioClient risposta;
        risposta.tipo= OK_TO_SEND;

        result= msgsnd(id_coda_mess, (void *)&risposta, sizeof(MessaggioClient)-sizeof(long), 0);
        if(result<0){perror("[SERVER]: Errore nell'invio del messaggio K_TO_SND dal client\n"); exit(1); }
        
        
        MessaggioClient m;

        result= msgrcv(id_coda_mess, (void *)&m, sizeof(MessaggioClient)-sizeof(long), TIPO_MESS, 0);
        if(result<0){perror("[SERVER]: Errore nella ricezione di un messaggio dal client\n"); exit(1); }

        int buffer = /* TBD */m.buffer_destinazione;
        int value = /* TBD */m.valore;

        printf("[SERVER FRONTEND %d] Ricevuto richiesta (valore=%d, buffer=%d)\n", getpid(), value, buffer);


        /* TBD: Completare il passaggio dei parametri.
                Passare alla funzione i riferimenti allo
                i-esimo buffer e ai relativi semafori.
         */

        scrivi(/* TBD */sem[buffer], ls[buffer], value);
    }


    /* TBD: Attendere la terminazione dei processi figli e deallocare le risorse */
    for(int k=0; k<NUM_SERVER; k++){
        wait(NULL);
        printf("[SERVER]: Terminazione del Worker n. %d\n", k+1);
    }

    for(int j= 0; j<NUM_BUFFER; j++){
        semctl(sem[j], 0, IPC_RMID);
        shmctl(shm[j], IPC_RMID, NULL);
    }



    return 0;
}