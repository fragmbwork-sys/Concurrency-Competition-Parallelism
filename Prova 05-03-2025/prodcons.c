#include "header_prodcons.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

#include <stdio.h>

void buffer_init(struct buffer * b) {

    /* TBD: Inizializzare la struttura dati */
    b->valore= 0;
    b->buffer_pieno= 0;
    b->buffer_vuoto= 1;

    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cv_cons, NULL);
    pthread_cond_init(&b->VAR_PROD, NULL);
}

void buffer_produci(struct buffer * b, int valore) {

    /* TBD: Completare la sincronizzazione */
    pthread_mutex_lock(&b->mutex);

    while(b->buffer_pieno==1){
        printf("[PRODUCI]: Non posso ancora produrre, buffer pieno\n");
        pthread_cond_wait(&b->VAR_PROD, &b->mutex);
    }

    printf("[PRODUCI]: Sono uscito dalla wait\n");


    b->valore = valore;
    b->buffer_pieno= 1;
    b->buffer_vuoto= 0;

    pthread_cond_signal(&b->cv_cons);

    pthread_mutex_unlock(&b->mutex);
}

int buffer_consuma(struct buffer * b) {

    int valore;

    /* TBD: Completare la sincronizzazione */
    pthread_mutex_lock(&b->mutex);

    while(b->buffer_vuoto==1){

        printf("[CONSUMA]: Non posso ancora consumare, non c'e' niente\n");
        pthread_cond_wait(&b->cv_cons, &b->mutex);
    }

    printf("[CONSUMA]: Sono uscito dalla wait\n");

    valore = b->valore;
    b->buffer_vuoto= 1;
    b->buffer_pieno= 0;

    pthread_cond_signal(&b->VAR_PROD);

    pthread_mutex_unlock(&b->mutex);

    return valore;
}

void buffer_destroy(struct buffer * b) {

    /* TBD: De-inizializzare le variabili per la sincronizzazione */
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->VAR_PROD);
    pthread_cond_destroy(&b->cv_cons);
}
