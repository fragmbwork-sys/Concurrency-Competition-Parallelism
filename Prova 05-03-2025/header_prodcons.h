#ifndef _HEADER_PRODCONS
#define _HEADER_PRODCONS

//include <bits/pthreadtypes.h>
#include <pthread.h>

struct buffer {
    
    int valore;

    /* TBD: Aggiungere altre variabili per la sincronizzazione */
    int buffer_pieno;
    int buffer_vuoto;

    pthread_mutex_t mutex;
    pthread_cond_t VAR_PROD;
    pthread_cond_t cv_cons;
};

void buffer_init(struct buffer * b);
void buffer_produci(struct buffer * b, int valore);
int buffer_consuma(struct buffer * b);
void buffer_destroy(struct buffer * b);

#endif