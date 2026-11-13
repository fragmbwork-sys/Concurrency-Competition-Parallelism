#ifndef _HEADER_PRODCONS_H_
#define _HEADER_PRODCONS_H_

//#include <bits/pthreadtypes.h>
#include <pthread.h>

typedef struct {

    /* TBD: Aggiungere ulteriori variabili per la sincronizzazione */
    pthread_cond_t VAR_PROD;
    pthread_cond_t VAR_CONS;
    pthread_mutex_t  mutex;

    int valore;
    int stato;

} BufferProdCons;

#define VUOTO 0
#define PIENO 1

void produci_valore(BufferProdCons * b, int valore);
int consuma_valore(BufferProdCons * b);
void * visualizza(void *);

#endif