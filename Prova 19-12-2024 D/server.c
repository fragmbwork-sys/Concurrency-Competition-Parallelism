#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "header.h"

int main() {

    /*
        TODO: recupero code e inizializzazione strutture
    */

    int key_richieste= ftok("./", 'a');
    int coda_richieste= msgget(key_richieste, 0);

    int key_risposte= ftok("./", 'z');
    int coda_risposte= msgget(key_risposte, 0);

    if (coda_richieste == -1 || coda_risposte == -1) {
        perror("Errore nell'apertura delle code");
        exit(1);
    }



    int id_shm= shmget(IPC_PRIVATE, sizeof(MonitorLettScritt), IPC_CREAT|0664);

    if(id_shm<0){
        printf("[WARNING_SERVER]: Errore nell'ottenimento della chiave della shared memory\n");
        exit(1);
    }


    MonitorLettScritt *p= shmat(id_shm, NULL, 0);

    inizializza_monitor(p);

    if(p== (void*)-1){
        printf("[WARNING_SERVER]: Errore nell'attach della shared memory alla risorsa condivisa\n");
        exit(1);
    }

    pid_t pids, pidl;

    for (int i=0; i<NUM_BAMBINI*NUM_LETTERE; i++) {
        /*
            TODO: creare figli
        */
       pids=fork();
            if(pids==0){
                printf("[SERVER]: Creazione del processo scrittore numero %d\n", i+1);
                Scrittore(coda_richieste, p);
                exit(0);
            }else if(pids<0){
                printf("[WARNING_SERVER]: Errore nella creazione dell processo figlio scrittore n. %d\n", i+1);
                exit(1);
            }
            sleep(rand()%3 + 1);

    }

    for (int i=0; i<NUM_BAMBINI*NUM_LETTERE; i++) {
        /*
            TODO: creare figli
        */
       pidl=fork();
            if(pidl==0){
                printf("[SERVER]: Creazione del processo lettore numero %d\n", i+1);
                Lettore(coda_risposte, p);
                exit(0);
            }else if(pidl<0){
                printf("[WARNING_SERVER]: Errore nella creazione dell processo figlio lettore n. %d\n", i+1);
                exit(1);
            }
            sleep(rand()%3 + 1);

    }

    /*
        TODO: attesa terminazione figli
    */
   //while(wait(NULL)>0);

   for(int k=0; k<30; k++){
    printf("[SERVER Terminazione del processo figlio n. %d\n", k+1);
    wait(NULL);
   }

    /*
        TODO: distruzione risorse
    */
   distruggi_monitor(p);
   shmctl(id_shm, IPC_RMID, 0);


    return 0;
}