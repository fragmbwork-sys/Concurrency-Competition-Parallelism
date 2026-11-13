#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "header.h"

int main() {
/*
    TODO: completare la creazione della memoria condivisa e inizializzazione struttura
*/
    int id_shm= shmget(IPC_PRIVATE, sizeof(MonitorPC), IPC_CREAT|0664);

    if(id_shm<0){
        printf("[MAIN]: Errore nella creazione della memoria condivisa\n");
        exit(1);
    }

    MonitorPC *p= shmat(id_shm, NULL, 0);
    inizializza_coda(p);

/*
    TODO: completare la creazione della coda di messaggi
*/
   int id_msgqueue= msgget(IPC_PRIVATE, IPC_CREAT|0664);

        if(id_msgqueue<0){

            printf("[MAIN]: Errore nella creazione della coda dei messaggi\n");
            exit(1);
        }

    /*int id_msgsincrone= msgget(IPC_PRIVATE, IPC_CREAT|0664);

        if(id_msgsincrone<0){

            printf("[MAIN]: Errore nella creazione della coda sincrona\n");
            exit(1);
        }*/

    p->id_coda= id_msgqueue;

/*
    TODO: figlio consumatore
*/
        pid_t pid=fork();

        if(pid==0){

            printf("[MAIN]: Creazione del processo figlio consumatore... \n");
            Consumatore(p, id_msgqueue);
            exit(0);
        }else if(pid<0){

            printf("[MAIN]: Errore nella creazione del processo figlio consumatore\n");
            exit(1);
        }


    pid_t pidp;
    for (int i = 0; i < NUM_BAMBINI; i++) {
        /*
            TODO: completare la creazione dei figli produttori
        */  pidp= fork();
        if (pidp==0){

            printf("[MAIN]: Creazione del processo figlio produttore numero %d ...\n", i+1);
            Produttore(p, i+1, id_msgqueue);
            exit(0);
        }else if(pidp<0){

            printf("[MAIN]: Errore nella creazione del processo Produttore...\n");
            exit(1);
        }
            //srand(time(NULL)*getpid());

        sleep(rand() % 3 + 1);
    }

    /*
        TODO: attesa terminazione processi e rimozione strutture
    */
   for(int k=0; k<NUM_BAMBINI+1; k++){

        printf("[MAIN]: Processo figlio numero %d terminato.\n", k+1);
        wait(NULL);
   }

   //remove_monitor(p);
   rimuovi_coda(p);
   msgctl(id_msgqueue, IPC_RMID, 0);
   //msgctl(id_msgsincrone, IPC_RMID, 0);
   shmctl(id_shm, IPC_RMID, 0);

    return 0;
}
