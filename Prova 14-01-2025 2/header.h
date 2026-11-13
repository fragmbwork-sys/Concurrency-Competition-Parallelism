#ifndef _HEADER_H
#define _HEADER_H


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#define RICHIESTA 1
#define RISPOSTA 2

typedef struct {

    /* TBD: Definire la struttura del messaggio di richiesta.
            La stessa struttura può essere usata sia per i messaggi dal client al server,
            sia per i messaggi dal server al database.
     */
    long tipo;

    int valoreM;
    int posizione;
    int identificativo;

} messaggio_richiesta;

typedef struct {

    /* TBD: Definire la struttura del messaggio di risposta.
            La stessa struttura può essere usata sia per i messaggi dal database al server,
            sia per i messaggi dal server al client.
     */
    long tipo_risp;

    int valore_letto;

} messaggio_risposta;



#define NUMERO_RICHIESTE 3

#define PROCESSI_CLIENT 3

#define PROCESSI_SERVER 3

#define DIM_VETTORE 3


#endif