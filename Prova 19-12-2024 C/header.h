#ifndef HEADER_H
#define HEADER_H

#include <bits/pthreadtypes.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define NUM_LETTERE 5
#define NUM_BAMBINI 3

/*
    TODO: completare le strutture necessarie per lo scambio di messaggi
*/
#define RICHIESTA 1
#define RISPOSTA 2

typedef struct {

    long tipo_richi;

    int numero_regali;

} MessaggioRichiesta;

typedef struct {

    long tipo_risp;

    int val;

} MessaggioRisposta;

typedef struct {
/*
    TODO: completare la struttura per risolvere il problema lettori-scrittori
*/

    pthread_mutex_t mutex;

    pthread_cond_t VAR_SCRITT;
    pthread_cond_t VAR_LETT;

    int num_lettori;
    int num_scritt;
    int num_scrittori_wait;

    int totale_regali;

} MonitorLettScritt;

#endif