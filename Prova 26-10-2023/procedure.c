#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void init_monitor(MonitorIO * m) {

    /* TBD: Inizializzare l'oggetto monitor */
    pthread_cond_init(&m->VAR_CONS, NULL);
    pthread_cond_init(&m->VAR_PROD, NULL);
    pthread_cond_init(&m->cv_attesa, NULL);

    pthread_mutex_init(&m->mutex, NULL);

    m->num_liberi=DIM;
    m->num_occupati=0;

    for(int i=0; i<DIM; i++){
        m->stato[i]= LIBERO;
    }
}

void end_monitor(MonitorIO * m) {

    /* TBD: Disattivare le variabili membro dell'oggetto monitor */
    pthread_cond_destroy(&m->VAR_CONS);
    pthread_cond_destroy(&m->VAR_PROD);
    pthread_cond_destroy(&m->cv_attesa);

    pthread_mutex_destroy(&m->mutex);
}

void produzione(MonitorIO * m, char c) {

    int i;

    pthread_mutex_lock(&m->mutex);
    
    
    /* TBD: Porre il thread in sospensione se il vettore di buffer è pieno */
    while(m->num_liberi==0){
        printf("[PRODUZIONE]: Non posso ancora produrre, buffer pieno\n");
        pthread_cond_wait(&m->VAR_PROD, &m->mutex);
    }

    printf("[NOTIFICA_PRODUZIONE]: Sono uscito dalla wait\n");


    /* TBD: Ricercare un elemento "i" nello stato LIBERO,
            in base allo schema con vettore di stato       */

    i=0;

    while(i<DIM && m->stato[i]!=LIBERO){
    i++;
    }

    m->stato[i]= IN_USO;
    m->num_liberi--;

    pthread_mutex_unlock(&m->mutex);

    
    printf("PRODUZIONE: inizio produzione vettore[%d]\n", i);
    sleep(1 + (rand() % 3));
    m->vettore[i] = c;
    printf("PRODUZIONE: vettore[%d] <- '%c'\n", i, c);
    

    pthread_mutex_lock(&m->mutex);

    m->stato[i]= OCCUPATO;
    m->num_occupati++;

    
    /* TBD: Riattivare un thread consumatore sospeso */
    pthread_cond_signal(&m->VAR_CONS);

    /* TBD: Riattivare i thread sospesi sulla variabile condition "cv_attesa" */
    pthread_cond_broadcast(&m->cv_attesa);

    pthread_mutex_unlock(&m->mutex);


    
}

char consumazione(MonitorIO * m) {

    int i;

    /* TBD: Porre il thread in sospensione se il vettore di buffer è vuoto */
    pthread_mutex_lock(&m->mutex);

    while(m->num_occupati==0){
        printf("[CONSUMAZIONE]: Non posso ancora consumare, buffer vuoto\n");
        pthread_cond_wait(&m->VAR_CONS, &m->mutex);
    }

    printf("[NOTIFICA_CONSUMAZIONE]: Sono uscito dalla wait\n");


    /* TBD: Ricercare un elemento "i" nello stato OCCUPATO,
            in base allo schema con vettore di stato       */

    i=0;

    while(i<DIM && m->stato[i]!= OCCUPATO){
    i++;
    }

    m->stato[i]= IN_USO;
    m->num_occupati--;

    pthread_mutex_unlock(&m->mutex);



    printf("CONSUMAZIONE: inizio consumazione vettore[%d]\n", i);
    sleep(1);
    char c = m->vettore[i];
    printf("CONSUMAZIONE: vettore[%d] -> '%c'\n", i, c);


    pthread_mutex_lock(&m->mutex);

    m->stato[i]= LIBERO;
    m->num_liberi++;

    /* TBD: Riattivare un thread produttore sospeso */
    pthread_cond_signal(&m->VAR_PROD);

    pthread_mutex_unlock(&m->mutex);

    return c;
}

int attesa(MonitorIO * m) {

    int occupati;

    /* TBD: Se il numero di buffer occupati è minore di 3,
     *      sospendere il thread sulla condition variable "cv_attesa" */

    pthread_mutex_lock(&m->mutex);

    while(m->num_occupati<3){
        printf("[NOTIFICA_ATTESA]: Non sono ancora stati prodotti 3 elementi\n");
        pthread_cond_wait(&m->cv_attesa, &m->mutex);
    }

    if(m->num_occupati>=3){
        printf("[ATTESA]: Sono stati prodotti almeno 3 elementi\n");
        occupati = /* TBD: Restituire in uscita il numero di buffer occupati */m->num_occupati;
    }

    pthread_mutex_unlock(&m->mutex);

    return occupati;
}
