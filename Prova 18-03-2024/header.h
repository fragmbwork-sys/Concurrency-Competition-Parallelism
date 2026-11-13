#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define NTHREADS_PROD 8
#define NELEM_MONITOR 10
#define N_COMPUTE_PER_THREAD 5
#define REQUEST_RATE 4

struct coordinates {
    int x;
    int y;
};

struct monitor {
    int n_mex_lastsec; // numero messaggi prodotti nell'ultimo secondo, per rate limiting
    struct coordinates buffer[NELEM_MONITOR];
    
    //TODO: aggiungere gli altri campi necessari alla sincronizzazione

    int testa;
    int coda;

    pthread_mutex_t mutex;

    pthread_cond_t VAR_PRODUTT;
    pthread_cond_t VAR_CONS;

};

struct message{
    //TODO: completare il msg da mandare al plotter
    long tipo;
    struct coordinates valore;
};

struct parameters{
    //TODO: completare con i parametri da passare ai thread produttori
    struct monitor *m;
    int x; //coordinata x da passare 

};

void inizializza(struct monitor * m);
void rimuovi (struct monitor * m);
void send_coordinates(struct monitor* m, struct coordinates* c);
void receive_coordinates(struct monitor* m, struct coordinates* c);
void reset_rate(struct monitor* m);


#endif
