#include <pthread.h>
#include <stdio.h>

#include "prodcons.h"


void init_monitor(MonitorProdCons * p) {

    /* TBD: Inizializzare l'oggetto monitor */
    p->nelem= 0;
    p->coda= 0;
    p->testa= 0;

    p->conteggio_consumazioni = 0;

    pthread_cond_init(&p->VAR_PROD, NULL);
    pthread_cond_init(&p->VAR_CONS, NULL);
    pthread_cond_init(&p->VAR_THREAD, NULL);
    pthread_mutex_init(&p->mutex, NULL);
}

void produzione(MonitorProdCons * p, int val) {

    /* TBD: Completare la sincronizzazione, in base allo schema
            del vettore di buffer circolare */
    pthread_mutex_lock(&p->mutex);

    while(p->nelem==DIM){
        printf("[PRODUZIONE]: Non posso ancora produrre, buffer pieno\n");
        pthread_cond_wait(&p->VAR_PROD, &p->mutex);
    }

    printf("[PRODUZIONE]: Sono uscito dall'attesa\n");

    p->buffer[/*TBD*/p->coda] = val;
    p->coda= (p->coda+1)%DIM;
    p->nelem++;

    pthread_cond_signal(&p->VAR_CONS);

    pthread_mutex_unlock(&p->mutex);

}

int consumazione(MonitorProdCons * p) {

    int val;

    /* TBD: Completare la sincronizzazione, in base allo schema
            del vettore di buffer circolare */
    
    pthread_mutex_lock(&p->mutex);

    while(p->nelem==0){
        printf("[CONSUMAZIONE]: Non posso consumare, buffer vuoto\n");
        pthread_cond_wait(&p->VAR_CONS, &p->mutex);
    }

    printf("[CONSUMAZIONE]: Sono uscito dall'attesa\n");

    val = p->buffer[/*TBD*/p->testa];
    p->testa= (p->testa+1)%DIM;
    p->nelem--;

    p->conteggio_consumazioni++;

    //if(p->conteggio_consumazioni>=5){ NON LA POSSO METTERE PERCHE' NON HO LA VARIABILE CONTEGGIO A DISPOSIZIONE CHE NON E' SEMPRE 5
        //printf("[NOTIFICA_CONSUMAZIONE]: La variabile conteggio ha raggiunto le 5 consumazioni\n");
        pthread_cond_signal(&p->VAR_THREAD);
    //}

    pthread_cond_signal(&p->VAR_PROD);

    pthread_mutex_unlock(&p->mutex);

    return val;
}

int attendi_consumazioni(MonitorProdCons * p, int soglia) {

    int n;

    /* TBD: Sospendere il thread finché il "conteggio_consumazioni" non sia diventato
            maggiore o uguale a "soglia"
     */
     pthread_mutex_lock(&p->mutex);

    while(p->conteggio_consumazioni<soglia){
        pthread_cond_wait(&p->VAR_THREAD, &p->mutex);
    }
    
    printf("[NOTIFICA_ATTENDI]: Sono stato svegliato\n");


    n = p->conteggio_consumazioni;

    p->conteggio_consumazioni = 0;
    
    pthread_mutex_unlock(&p->mutex);

    return n;
}

void remove_monitor(MonitorProdCons * p) {

    /* TBD: Disattivare le variabili per la sincronizzazione */
    pthread_cond_destroy(&p->VAR_CONS);
    pthread_cond_destroy(&p->VAR_PROD);
    pthread_cond_destroy(&p->VAR_THREAD);

    pthread_mutex_destroy(&p->mutex);
}