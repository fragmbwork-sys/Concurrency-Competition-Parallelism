//#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "prodcons.h"

#define NUM_THREAD_PROD 3
#define NUM_THREAD_CONS 3
#define NUM_PRODUZIONI 10
#define NUM_CONSUMAZIONI 10
#define CONTEGGIO 5

void * thread_produttore(void *);
void * thread_consumatore(void *);
void * thread_conteggio(void *);


int main() {

    /* TBD: Creare un oggetto monitor di tipo "MonitorProdCons" */
    MonitorProdCons *mon= (MonitorProdCons*) malloc(sizeof(MonitorProdCons));

    init_monitor(/*TBD*/mon);

    pthread_t id_threads_prod[NUM_THREAD_PROD];
    pthread_t id_threads_cons[NUM_THREAD_CONS];
    pthread_t id_thread_conteggio;

    for(int i=0; i<NUM_THREAD_PROD; i++) {

        /* TBD: Creare un thread produttore, fargli eseguire la funzione "thread_produttore" */
        printf("[MAIN]: Creo il Thread Produttore n. %d\n", i+1);
        //monitor[i]= mon;
        int err= pthread_create(&id_threads_prod[i], NULL, thread_produttore, (void *)mon);

        if(err){
            perror("[MAIN]: Errore nella creazione di un Thread Produttore\n");
            exit(1);
        }
    }

    for(int i=0; i<NUM_THREAD_CONS; i++) {

        /* TBD: Creare un thread consumatore, fargli eseguire la funzione "thread_consumatore" */
        printf("[MAIN]: Creo il Thread Consumatore n. %d\n", i+1);
        //monitor[i]= mon;
        int err= pthread_create(&id_threads_cons[i], NULL, thread_consumatore, (void *)mon);

        if(err){
            perror("[MAIN]: Errore nella creazione di un Thread Consumatore\n");
            exit(1);
        }
    }


    /* TBD: Creare un thread di "conteggio", fargli eseguire la funzione "thread_conteggio" */
    printf("[MAIN]: Creo il Thread Conteggio...\n");
        
        int err= pthread_create(&id_thread_conteggio, NULL, thread_conteggio, (void *)mon);

        if(err){
            perror("[MAIN]: Errore nella creazione del Thread Conteggio\n");
            exit(1);
        }


    /* TBD: Attendere la terminazione dei thread */
    for(int k=0; k<NUM_THREAD_PROD; k++){
        pthread_join(id_threads_prod[k], NULL);
        printf("[MAIN]: Terminazione del Thread Produttore n. %d\n", k+1);
    }

    for(int k=0; k<NUM_THREAD_CONS; k++){
        pthread_join(id_threads_cons[k], NULL);
        printf("[MAIN]: Terminazione del Thread Consumatore n. %d\n", k+1);
    }

    
        pthread_join(id_thread_conteggio, NULL);
        printf("[MAIN]: Terminazione del Thread Conteggio\n");
    

    remove_monitor(/* TBD */mon);

    /* TBD: Deallocare l'oggetto monitor */
    free(mon);

}

void * thread_produttore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons *mon= (MonitorProdCons*) x;

    for(int i=0; i<NUM_PRODUZIONI; i++) {

        /* TBD: Completare la chiamata a produzione() */
        int val = rand() % 10;
        produzione(/*TBD*/mon, val);

        printf("[PRODUTTORE] Ho prodotto: %d\n", val);

    }

    return NULL;
}

void * thread_consumatore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons* mon= (MonitorProdCons*) x;

    for(int i=0; i<NUM_CONSUMAZIONI; i++) {

        /* TBD: Completare la chiamata a consumazione() */

        int val = consumazione(/*TBD*/mon);

        printf("[CONSUMATORE] Ho consumato: %d\n", val);
    }

    return NULL;
}


void * thread_conteggio(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons* mon= (MonitorProdCons*) x;

    int consumazioni_effettuate = 0;

    int consumazioni_da_effettuare = NUM_CONSUMAZIONI * NUM_THREAD_CONS;

    while(consumazioni_da_effettuare > 0) {

        int conteggio;

        if( consumazioni_da_effettuare >= CONTEGGIO ) {

            // Vi sono 5 o più consumazioni ancora da effettuare,
            // si pone a 5 il conteggio da attendere
            conteggio = CONTEGGIO;
        }
        else {

            // Vi sono meno di 5 consumazioni ancora da effettuare,
            // si pone il conteggio da attendere a "consumazioni_da_effettuare"
            conteggio = consumazioni_da_effettuare;
        }

        /* TBD: Completare la chiamata ad "attendi_consumazioni()" */

        int n = attendi_consumazioni(/* TBD */mon, conteggio);

        consumazioni_effettuate += n;

        consumazioni_da_effettuare -= n;

        printf("[CONTEGGIO] Sono stati consumati in totale %d elementi\n", consumazioni_effettuate);
    }

    return NULL;
}
