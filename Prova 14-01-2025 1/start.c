#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#include "monitor_fifo.h"

#define PROCESSI_PRODUTTORI 5
#define PROCESSI_CONSUMATORI 2

#define NUMERO_PRODUZIONI 4
#define NUMERO_CONSUMAZIONI 10

void consumatore(MonitorProdConsFIFO * p);
void produttore(MonitorProdConsFIFO * p);

int main() {

    /* TBD: Creare l'oggetto monitor, utilizzando la funzione "inizializza()" */
    int id_shm= shmget(IPC_PRIVATE, sizeof(MonitorProdConsFIFO), IPC_CREAT|0664);

    if(id_shm< 0){

        printf("[START]: Errore nella creazione della memoria condivisa\n");
        exit(-1);
    }

    MonitorProdConsFIFO *variabile_cond= shmat(id_shm, NULL, 0);

    inizializza(variabile_cond);

    /* TBD: Avviare i processi produttori, facendogli eseguire la funzione "produttore()"
            e i processi consumatori, facendogli eseguire la funzione "consumatore()"
     */
    for(int i= 0; i<PROCESSI_PRODUTTORI; i++){

        pid_t pid= fork();

        if(pid==0){

            produttore(variabile_cond);
            exit(1);
        
        }else if(pid <0){

            printf("[START]: Errore nella creazione del processo figlio produttore\n");
            exit(-1);
        }

    }


    for(int j= 0; j<PROCESSI_CONSUMATORI; j++){

        pid_t pid= fork();

        if(pid==0){

            consumatore(variabile_cond);
            exit(1);
        
        }else if(pid <0){

            printf("[START]: Errore nella creazione del processo figlio consumatore\n");
            exit(-1);
        }

    }

        
    for(int i=0; i< PROCESSI_PRODUTTORI+PROCESSI_CONSUMATORI; i++){

        wait(NULL);
    }

    /* TBD: Rimuovere l'oggetto monitor, utilizzando la funzione "distruggi()" */
    distruggi(variabile_cond);

    shmctl(id_shm, IPC_RMID, 0);
}


void produttore(MonitorProdConsFIFO * p) {

    srand(getpid());

    for(int i=0; i<NUMERO_PRODUZIONI; i++) {

        int valore = rand() % 100;

        printf("[PRODUTTORE %d] Produzione valore %d\n", getpid(), valore);

        /* TBD: Chiamare la funzione "produci()" del monitor */
        produci(p, valore);

    }

}

void consumatore(MonitorProdConsFIFO * p) {

    for(int i=0; i<NUMERO_CONSUMAZIONI; i++) {

        printf("[CONSUMATORE %d] Inizio consumazione\n", getpid());

        int valore;

        /* TBD: Chiamare la funzione "consuma()" del monitor */
        valore= consuma(p);

        printf("[CONSUMATORE %d] Consumazione valore %d\n", getpid(), valore);
    }

}