#ifndef _HEADER_
#define _HEADER_

#include "monitor_hoare.h"
#include "semafori.h"

#define DIM 3

/* TBD: Aggiungere le variabili per la sincronizzazione con 
     *      l'algoritmo del vettore di stato
     */

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2

#define VAR_PROD 0
#define VAR_CONS 1

typedef struct {

    int buffer[DIM];

    /* TBD: Aggiungere le variabili per la sincronizzazione con 
     *      l'algoritmo del vettore di stato
     */
     int stato[DIM];
     int nelem_liberi;
     int nelem_occupati;

     Monitor m;

} VettoreProdCons;

typedef struct {

    int buffer;

    int sem_id;  // Utilizzare questa variabile per la mutua esclusione

} BufferMutuaEsclusione;

#define MUTEX 0

void inizializza_vettore(VettoreProdCons * p);
void produci(VettoreProdCons * p, int valore);
int consuma(VettoreProdCons * p);
void rimuovi_vettore(VettoreProdCons * p);

void inizializza_buffer(BufferMutuaEsclusione * p);
void aggiorna(BufferMutuaEsclusione * p, int valore);
void rimuovi_buffer(BufferMutuaEsclusione * p);


#endif
