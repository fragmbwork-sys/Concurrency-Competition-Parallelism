//#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "header.h"

CodaCircolare coda;


int main() {
    srand(time(NULL));

    /*
        TODO: allocazione buffer e inizializzazione strutture
    */
    BufferBabbo *buffer= (BufferBabbo*)malloc(sizeof(BufferBabbo));


    inizializza_buffer(buffer);
    inizializza_coda(&coda);


    /*
        TODO: creazione thread produttori, consumatori e Babbo_Natale
    */

    pthread_t id_produttori[NPROD];
    pthread_t id_consumatori[NCONS];
    pthread_t id_thread_babbo;

    for(int i=0; i<NPROD; i++){
        
        printf("[MAIN]: Creazione del Produttore n. %d\n", i+1);
        int err= pthread_create(&id_produttori[i], NULL, Produttore, (void*)&coda);

        if(err){perror("[MAIN]: Errore nella creazione di un Thread Consumatore\n"); exit(1);}

    }

    for(int i=0; i<NCONS; i++){
        
        printf("[MAIN]: Creazione del Consumatore n. %d\n", i+1);
        int err= pthread_create(&id_consumatori[i], NULL, Consumatore, (void*)buffer);

        if(err){perror("[MAIN]: Errore nella creazione di un Thread Consumatore\n"); exit(1);}

    }

    printf("[MAIN]: Creazione del Babbo\n");
    pthread_create(&id_thread_babbo, NULL, Babbo_Natale, (void*)buffer);


    /*
        TODO: attesa terminazione thread produttori, consumatori e Babbo_Natale
    */
    for(int k=0; k<NPROD; k++){
        
        pthread_join(id_produttori[k], NULL);
        printf("[MAIN]: Terminazione del Thread Produttore n. %d\n", k+1);
    }

    for(int k=0; k<NCONS; k++){
        
        pthread_join(id_consumatori[k], NULL);
        printf("[MAIN]: Terminazione del Thread Consumatore n. %d\n", k+1);
    }

    pthread_join(id_thread_babbo, NULL);

    /*
        TODO: rimozione strutture
    */

    distruggi_coda(&coda);
    distruggi_buffer(buffer);

    free(buffer);

    return 0;
}

void* Produttore(void* p) {

    CodaCircolare *coda= (CodaCircolare*) p;
    for (int i = 0; i < GIOCATTOLI_PER_PROD; i++) {
        sleep(2);
        int giocattolo = rand() % 100 + 1;
        /*
            TODO: produzione e terminazione
        */
        produci(coda, giocattolo);
    }

    pthread_exit(NULL);

}


// Thread consumatore
void* Consumatore(void* p) {

    BufferBabbo *b= (BufferBabbo*) p;

    for (int i=0; i<PRELIEVI_PER_CONS; i++) {
        /*
            TODO: consumo, aggiornamento e terminazione
        */
        consuma(&coda);
        aggiorna_conteggio(b);
    }

    pthread_exit(NULL);

}

// Thread Babbo Natale
void* Babbo_Natale(void* p) {

    BufferBabbo *b= (BufferBabbo*) p;

    for (int i=0; i<CONSEGNE; i++) {
        /*
            TODO: consegna e terminazione
        */
        consegna(b);
    }

    pthread_exit(NULL);

}