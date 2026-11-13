#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "header.h"
#include <wait.h>

#define NUM_PRODUTTORI 2
#define NUM_CONSUMATORI 5

#define PRODUZIONI 10
#define CONSUMAZIONI 4

void produttore(VettoreProdCons * vettore);
void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer);

int main() {

    int id_prodcons= shmget(IPC_PRIVATE, sizeof(VettoreProdCons), IPC_CREAT|0664);
    int id_buffer= shmget(IPC_PRIVATE, sizeof(BufferMutuaEsclusione), IPC_CREAT|0664);

    VettoreProdCons * vettore = /* TBD: Allocare la struttura dati */(VettoreProdCons*)shmat(id_prodcons, NULL, 0);

    BufferMutuaEsclusione * buffer = /* TBD: Allocare la struttura dati */(BufferMutuaEsclusione*)shmat(id_buffer, NULL, 0);

    if(vettore==(void*)-1){perror("[MAIN]: errore nella creazione della memoria condivisa PRODCONS\n"); exit(1);}
    if(buffer==(void*)-1){perror("[MAIN]: errore nella creazione della memoria condivisa BUFFER\n"); exit(1);}

    inizializza_vettore(vettore);
    inizializza_buffer(buffer);


    for(int i=0; i<NUM_PRODUTTORI; i++) {

        /* TBD: Creare i processi produttori, e fargli chiamare la
         *      funzione "produttore()"
         */
         pid_t pid=fork();
         if(pid==0){
            printf("[MAIN]: Creazione del processo produttore n. %d\n", i+1);
            produttore(vettore);
            exit(0);
         }else if(pid<0){printf("[MAIN]: Errore con il pid del produttore n. %d\n",i+1); exit(1);}
    }


    for(int i=0; i<NUM_CONSUMATORI; i++) {

        /* TBD: Creare i processi consumatori, e fargli chiamare la
         *      funzione "consumatore()"
         */
         pid_t pid=fork();
         if(pid==0){
            printf("[MAIN]: Creazione del processo consumatore n. %d\n", i+1);
            consumatore(vettore, buffer);
            exit(0);
         }else if(pid<0){printf("[MAIN]: Errore con il pid del consumatore n. %d\n",i+1); exit(1);}
    }


    /* TBD: Attendere la terminazione dei processi figli */
    for(int k=0; k<NUM_PRODUTTORI+NUM_CONSUMATORI; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);
    }


    rimuovi_buffer(buffer);
    rimuovi_vettore(vettore);


    /* TBD: De-allocare le strutture dati */
    shmctl(id_prodcons, IPC_RMID, NULL);
    shmctl(id_buffer, IPC_RMID, NULL);

    return 0;

}

void produttore(VettoreProdCons * vettore) {

    srand(getpid());
    
    for(int i=0; i<PRODUZIONI; i++) {

        int valore = rand() % 10;

        printf("[MAIN PRODUTTORE] Produzione: %d\n", valore);

        /* TBD: Chiamare il metodo "produci()" del monitor */
        produci(vettore, valore);
    }

}

void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer) {

    srand(getpid());

    for(int i=0; i<CONSUMAZIONI; i++) {

        int valore;

        /* TBD: Chiamare il metodo "consuma()" del monitor */
        valore= consuma(vettore);

        printf("[MAIN CONSUMATORE] Consumazione: %d\n", valore);

        /* TBD: Chiamare il metodo "aggiorna()", passandogli 
         *      il valore che è stato appena consumato
         */
         aggiorna(buffer, valore);
    }
}
