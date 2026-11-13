#include "header.h"

void inizializza(struct monitor* m){

    // TODO: inizializza variabile di supporto per rate limiting e buffer circolare
    m->testa= 0;
    m->coda= 0;
    m->n_mex_lastsec= 0;
    for(int i=0; i<NELEM_MONITOR; i++){
        m->buffer[i].x= 0;
        m->buffer[i].y= 0;
    }

    // TODO: inizializza mutex e cv necessarie
    pthread_mutex_init(&m->mutex, NULL);
    pthread_cond_init(&m->VAR_PRODUTT, NULL);
    pthread_cond_init(&m->VAR_CONS, NULL);
}

void rimuovi (struct monitor* m){
    // TODO: rimuovi strutture inizializzate
    pthread_mutex_destroy(&m->mutex);
    pthread_cond_destroy(&m->VAR_CONS);
    pthread_cond_destroy(&m->VAR_PRODUTT);

}

void send_coordinates(struct monitor* m, struct coordinates* c){
    //TODO: entra nel monitor e deposita le coordinate se si è sotto al limite di messaggi al secondo

    pthread_mutex_lock(&m->mutex);

    while(m->n_mex_lastsec>=4 || ((m->testa+1)%NELEM_MONITOR)==m->coda){
        printf("[PRODUTTORE]: Non posson produrre perche' buffer pieno o per troppe produzioni\n");
        pthread_cond_wait(&m->VAR_PRODUTT, &m->mutex);
    }

    m->buffer[m->testa].x= c->x;
    m->buffer[m->testa].y= c->y;
    m->testa= (m->testa+1)%NELEM_MONITOR;

    m->n_mex_lastsec++;

    pthread_cond_signal(&(m->VAR_CONS));

    pthread_mutex_unlock(&(m->mutex));


}

void receive_coordinates(struct monitor* m, struct coordinates* c){
    //TODO: entra nel monitor e consuma

    pthread_mutex_lock(&(m->mutex));

    while(m->coda==m->testa){
        printf("[CONSUMATORE]: Non ci sono ancora elementi da consuamre, attendo\n");
        pthread_cond_wait(&(m->VAR_CONS), &(m->mutex));
    }

    c->x= m->buffer[m->coda].x;
    c->y= m->buffer[m->coda].y;
    m->coda= (m->coda+1)%NELEM_MONITOR;

    pthread_cond_signal(&(m->VAR_PRODUTT));

    pthread_mutex_unlock(&(m->mutex));


}

void reset_rate(struct monitor* m){
    //TODO: reset rate
    pthread_mutex_lock(&(m->mutex));

    m->n_mex_lastsec= 0;

    pthread_cond_signal(&(m->VAR_PRODUTT));

    pthread_mutex_unlock(&(m->mutex));
}





