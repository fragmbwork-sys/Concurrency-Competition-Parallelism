#include "prodcons.h"

#include <pthread.h>
#include <stdio.h>


void inizializza(MonitorStreaming * m) {

    /* TBD: Inizializzare il monitor */
    m->nelem= 0;
    m->testa= 0;
    m->coda= 0;

    pthread_mutex_init(&m->mutex, NULL);
    pthread_cond_init(&m->VAR_PROD, NULL);
    pthread_cond_init(&m->VAR_CONS, NULL);
}

void produci(MonitorStreaming * m, Frame f) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    pthread_mutex_lock(&m->mutex);

    while(m->nelem==DIM){
        printf("[PRODUCI]: Non posso ancora produrre, buffer pieno\n");
        pthread_cond_wait(&m->VAR_PROD, &m->mutex);
    }

    printf("Produzione frame: { %d, %d, %d, %d }\n", f[0][0], f[0][1], f[1][0], f[1][1]);

    /* TBD */m->vettore[m->coda][0][0] = f[0][0];
    /* TBD */ m->vettore[m->coda][0][1] = f[0][1];
    /* TBD */ m->vettore[m->coda][1][0] = f[1][0];
    /* TBD */ m->vettore[m->coda][1][1] = f[1][1];
    m->coda= (m->coda+1)%DIM;
    m->nelem++;

    if(m->nelem>=4){
        printf("[PRODUCI]: Ho generato 4 elementi, sveglio il consumatore\n");
        pthread_cond_signal(&m->VAR_CONS);
    }

    pthread_mutex_unlock(&m->mutex);

}

void consuma(MonitorStreaming * m, Frame f) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    pthread_mutex_lock(&m->mutex);

    while(m->nelem==0){
        printf("[CONSUMA]: Non c'e' niente da consumare, attendo...\n");
        pthread_cond_wait(&m->VAR_CONS, &m->mutex);
    }

    f[0][0] = /* TBD */ m->vettore[m->testa][0][0];
    f[0][1] = /* TBD */ m->vettore[m->testa][0][1];
    f[1][0] = /* TBD */ m->vettore[m->testa][1][0];
    f[1][1] = /* TBD */ m->vettore[m->testa][1][1];
    m->testa= (m->testa+1)%DIM;
    m->nelem--;

    printf("Consumazione frame: { %d, %d, %d, %d }\n", f[0][0], f[0][1], f[1][0], f[1][1]);

    pthread_cond_signal(&m->VAR_PROD);

    pthread_mutex_unlock(&m->mutex);

}

void bufferizza(MonitorStreaming * m, int n) {

    /* TBD: Completare il metodo */
    pthread_mutex_lock(&m->mutex);

    while(m->nelem<4){
        pthread_cond_wait(&m->VAR_CONS, &m->mutex);
    }

    printf("[BUFFERIZZA]: Sono stati prodotti 4 frame!\n");


    pthread_mutex_unlock(&m->mutex);


}

void distruggi(MonitorStreaming * m) {

    /* TBD: Completare il metodo */
    pthread_cond_destroy(&m->VAR_CONS);
    pthread_cond_destroy(&m->VAR_PROD);

    pthread_mutex_destroy(&m->mutex);

}