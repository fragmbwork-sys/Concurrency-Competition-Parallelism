#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

void inizializza_coda(CodaCircolare *coda) {
    /*
        TODO: completare
    */
    coda->coda= 0;
    coda->testa= 0;
    coda->count= 0;

    for(int k=0; k<DIM; k++){
        coda->buffer_circolare[k]= 0;
    }
    
    pthread_mutex_init(&coda->mutex, NULL);
    pthread_cond_init(&coda->VAR_PROD, NULL);
    pthread_cond_init(&coda->VAR_CONS, NULL);
}

void distruggi_coda(CodaCircolare *coda) {
    /*
        TODO: completare
    */
    pthread_mutex_destroy(&coda->mutex);
    pthread_cond_destroy(&coda->VAR_CONS);
    pthread_cond_destroy(&coda->VAR_PROD);
}


void produci(CodaCircolare *coda, int giocattolo) {
    /*
        TODO: produzione
    */
    pthread_mutex_lock(&coda->mutex);

    while(coda->count>=DIM/*(coda->coda+1)%DIM==coda->testa*/){
        printf("[PRODUCI]: Non posso ancora produrre, buffer pieno\n");
        pthread_cond_wait(&coda->VAR_PROD, &coda->mutex);
    }

    printf("[PRODUCI]: Sono uscito dalla wait\n");

    coda->buffer_circolare[coda->coda]= giocattolo;
    coda->coda= (coda->coda+1)%DIM;
    coda->count++;

    printf("Prodotto giocattolo: %d (count=%d)\n", giocattolo, coda->count);

    if(coda->count>=3){
        printf("[PRODUCI]: Sono stati prodotti almeno 3 regali, risveglio un consumatore\n");
        pthread_cond_signal(&coda->VAR_CONS);
    }

    pthread_mutex_unlock(&coda->mutex);

}

void consuma(CodaCircolare *coda) {

    pthread_mutex_lock(&coda->mutex);

    while(coda->count<3){

        printf("[CONSUMA]: Non sono ancora stati prodotti almeno 3 valori, mi fermo...\n");
        pthread_cond_wait(&coda->VAR_CONS, &coda->mutex);
    }

    printf("[CONSUMA]: Sono uscito dalla wait\n");

    for(int i=0; i<3; i++){

         /*
        TODO: consumo di 3 giocattoli 
    */
    int giocattolo;
    giocattolo= coda->buffer_circolare[coda->testa];
    coda->testa= (coda->testa+1)%DIM;
    coda->count--;
    
        printf("[CONSUMA]: Prelevato giocattolo: %d (count=%d)\n", giocattolo, coda->count);
        pthread_cond_signal(&coda->VAR_PROD);
    }

    

    pthread_mutex_unlock(&coda->mutex);
    
}

void inizializza_buffer(BufferBabbo *buffer) {
    /*
        TODO: completare
    */
    buffer->buffer_regali= 0;
    buffer->consegne_effettuate= 0;
    
    pthread_mutex_init(&buffer->mutex2, NULL);
    pthread_cond_init(&buffer->VAR_BABBO, NULL);
}

void distruggi_buffer(BufferBabbo *buffer) {
    /*
        TODO: completare
    */
    pthread_mutex_destroy(&buffer->mutex2);
    pthread_cond_destroy(&buffer->VAR_BABBO);

}

void aggiorna_conteggio(BufferBabbo *buffer) {
    /*
        TODO: incremento buffer una volta consumati 3 giocattoli
    */
    pthread_mutex_lock(&buffer->mutex2);

    buffer->buffer_regali++;


    printf("Regalo pronto: %d\n", buffer->buffer_regali);

    if(buffer->buffer_regali>=5){
        printf("[AGGIORNA]: Abbiamo i 5 regali, notifico Babbo Natale\n");
        pthread_cond_signal(&buffer->VAR_BABBO);
    }


    pthread_mutex_unlock(&buffer->mutex2);


    
}

void consegna(BufferBabbo *buffer) {

    /*
        TODO: consegna dei regali una volta impacchettati 5 regali
    */
    pthread_mutex_lock(&buffer->mutex2);
    
    while(buffer->buffer_regali<5){
        printf("[BABBO_NATALE]: Attendo i 5 regali da consegnare\n");
        pthread_cond_wait(&buffer->VAR_BABBO, &buffer->mutex2);
    }
    

    buffer->buffer_regali -= 5;

    buffer->consegne_effettuate++; 

    printf("Babbo Natale consegna effettuata #%d\n", buffer->consegne_effettuate);
    
    pthread_mutex_unlock(&buffer->mutex2);
}
