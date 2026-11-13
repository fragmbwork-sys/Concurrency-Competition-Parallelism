#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "header.h"

void * produttore(void *);
void * consumatore(void *);

int main() {

    pthread_t thread_consumatore;
    pthread_t thread_produttori[3];

    /* TBD: Creare un oggetto monitor, e inizializzarlo con init_monitor() */
    MonitorIO *moni= (MonitorIO*)malloc(sizeof(MonitorIO));

    init_monitor(moni);

    /* TBD: Creare 3 thread produttori, chiamando la funzione produttore() */
    for(int i=0; i<NUM_PROD; i++){

        printf("[MAIN]: Creazione del Thread Produttore n. %d\n", i+1);
        int err=pthread_create(&thread_produttori[i], NULL, produttore, (void*)moni);

        if(err){perror("[WARNING_MAIN]: Erroe nella creazione del Thread Produttre\n"); exit(1);}

    }

    /* TBD: Creare 1 thread consumatore, chiamando la funzione consumatore() */
    printf("[MAIN]: Creazione del Thread Consuamtore \n");
        int err=pthread_create(&thread_consumatore, NULL, consumatore, (void*)moni);

        if(err){perror("[WARNING_MAIN]: Erroe nella creazione del Thread Consumatore\n"); exit(1);}


    /* TBD: Attendere la terminazione dei 3 thread produttori */
    for(int k=0; k<NUM_PROD; k++){
        pthread_join(thread_produttori[k], NULL);
        printf("[MAIN]: Terminazione del Thread Produttore n. %d\n", k+1);
    }

    /* TBD: Attendere la terminazione del thread consumatore */
    pthread_join(thread_consumatore, NULL);
    printf("[MAIN]: Terminazione del Thread Consumatore\n");

    /* TBD: Disattivare e deallocare l'oggetto monitor */
    end_monitor(moni);
    free(moni);

}


void * produttore(void * p) {

    printf("Avvio thread produttore...\n");

    MonitorIO *moni= (MonitorIO*) p;

    for(int i=0; i<5; i++) {

        sleep(1);

        // genera carattere casuale tra "a" e "z"
        char carattere = 97 + (rand() % 26);

        /* TBD: Chiamare il metodo "produzione()", passando l'oggetto monitor e
         *      la variabile "carattere" */
         produzione(moni, carattere);
    }

    return NULL;
}

void * consumatore(void * p) {

    printf("Avvio thread consumatore...\n");

    MonitorIO *moni= (MonitorIO*) p;

    int totale_consumazioni = 0;

    while(totale_consumazioni < 15) {

        int occupati = 1;
        
        if(totale_consumazioni < 13) {

            printf("CONSUMATORE: In attesa di elementi disponibili...\n");

            occupati = /* TBD: Chiamare il metodo "attesa()" */ attesa(moni);

            printf("CONSUMATORE: %d elementi disponibili\n", occupati);
        }

        for(int j=0; j<occupati; j++) {

            char carattere;
            
            carattere = /* TBD: Chiamare il metodo "consumazione()" */consumazione(moni);
        }

        totale_consumazioni += occupati;
    }

    return NULL;
}
