#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <unistd.h>

#include "header.h"
#include "semafori.h"

void inizializza_vettore(VettoreProdCons * p) {

    /* TBD: Inizializzare la struttura dati "VettoreProdCons" */
    p->nelem_liberi=  DIM;
    p->nelem_occupati= 0;
    
    for(int j=0; j<DIM; j++){
        p->buffer[j]= 0;
    }

    for(int l=0; l<DIM; l++){
        p->stato[l]= LIBERO;
    }

    init_monitor(&p->m, 2);

}

void produci(VettoreProdCons * p, int valore) {

    /* TBD: Implementare la sincronizzazione, mediante
     *      l'algoritmo del vettore di stato
     */
     enter_monitor(&p->m);

     if(p->nelem_occupati==DIM){
        printf("[PRODUCI]: Vettore pieno, attendo...\n");
        wait_condition(&p->m, VAR_PROD);
     }

     int i= 0;

     while(i<DIM && p->stato[i]!=LIBERO){
        i++;
     }

     p->stato[i]= IN_USO;
     p->nelem_occupati++;

     leave_monitor(&p->m);


     printf("[PRODUCI]: Produco il valore %d\n", valore);

     p->buffer[i]= valore;


     enter_monitor(&p->m);

     p->stato[i]= OCCUPATO;
     p->nelem_liberi--;

     signal_condition(&p->m, VAR_CONS);

     leave_monitor(&p->m);

}

int consuma(VettoreProdCons * p) {

    int valore;

    /* TBD: Implementare la sincronizzazione, mediante
     *      l'algoritmo del vettore di stato
     */

     enter_monitor(&p->m);

     if(p->nelem_liberi==DIM){
        printf("[CONSUMA]: Vettore ancora vuoto, attendo...\n");
        wait_condition(&p->m, VAR_CONS);
     }

     int i=0;

     while(i<DIM && p->stato[i]!=OCCUPATO){
        i++;
     }

     p->stato[i]= IN_USO;
     p->nelem_liberi++;

     leave_monitor(&p->m);

     printf("[CONSUMA]: Sto consumando un valore\n");

     valore= p->buffer[i];


     enter_monitor(&p->m);

     p->stato[i]= LIBERO;
     p->nelem_occupati--;

     signal_condition(&p->m, VAR_PROD);

     leave_monitor(&p->m);

    return valore;

}

void rimuovi_vettore(VettoreProdCons * p) {

    /* TBD: De-inizializzare la struttura dati "VettoreProdCons" */
    remove_monitor(&p->m);
}


void inizializza_buffer(BufferMutuaEsclusione * p) {

    /* TBD: Inizializzare la struttura dati "BufferMutuaEsclusione" */
    p->buffer= 0;

    p->sem_id= semget(IPC_PRIVATE, 1, IPC_CREAT|0664);

    semctl(p->sem_id, MUTEX, SETVAL, 1);
}

void aggiorna(BufferMutuaEsclusione * p, int valore) {

    /* TBD: Aggiungere la sincronizzazione per la mutua esclusione */
    Wait_Sem(p->sem_id, MUTEX);

    p->buffer += valore;

    printf("[AGGIORNA] Nuovo valore del buffer: %d\n", p->buffer);

    Signal_Sem(p->sem_id, MUTEX);
}

void rimuovi_buffer(BufferMutuaEsclusione * p) {

    /* TBD: De-inizializzare la struttura dati "BufferMutuaEsclusione" */
    semctl(p->sem_id, MUTEX, IPC_RMID);
}


