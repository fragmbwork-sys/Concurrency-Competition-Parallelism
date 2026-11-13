#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_prodcons.h"
#include "header_msg.h"

void server(int id_queue_server) {

    BufferProdCons * b = /* TBD: Creare e inizializzare l'oggetto monitor */(BufferProdCons*) malloc(sizeof(BufferProdCons));

    b->stato= VUOTO;
    b->valore= 0;
    pthread_cond_init(&b->VAR_CONS, NULL);
    pthread_cond_init(&b->VAR_PROD, NULL);
    pthread_mutex_init(&b->mutex, NULL);

    /* TBD: Creare un thread, 
            e fargli eseguire la funzione "visualizza()",
            passandogli in ingresso l'oggetto monitor
     */
     pthread_t id_visualizzatore;

    int err= pthread_create(&id_visualizzatore, NULL, visualizza, (void*)b);

    if(err){perror("[SERVER]: Errore nella creazione del Thread Visualizzatore\n"); exit(1);}


    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int somma_valori = 0;
        int media = 0;

        for(int j=0; j<NUM_COLLETTORI; j++) {

            int id_collettore = j+1;

            messaggio_collettore m;

            /* TBD: Ricevere un messaggio da un collettore, con ricezione selettiva */
            int result= msgrcv(id_queue_server, (void *)&m, sizeof(messaggio_collettore)-sizeof(long), id_collettore, 0);

            if(result<0){perror("[SERVER]: Errore nella ricezione di un messaggio\n"); exit(1);}

            int valore = /* TBD */m.valore_C;

            printf("[SERVER] Ricevuto valore=%d\n", valore);

            somma_valori += valore;

        }

        media = somma_valori / NUM_COLLETTORI;

        /* TBD: Chiamare la funzione "produci_valore()",
                passandogli la variabile "media"
        */
        produci_valore(b, media);

    }


    /* TBD: Attendere la terminazione del thread, de-allocare il monitor */
    pthread_join(id_visualizzatore, NULL);

    pthread_cond_destroy(&b->VAR_PROD);
    pthread_cond_destroy(&b->VAR_CONS);
    pthread_mutex_destroy(&b->mutex);

    free(b);

}

void * visualizza(void * p) {

    BufferProdCons* b= (BufferProdCons*) p;

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int valore = /* TBD: Utilizzare la funzione "consuma_valore()" */consuma_valore(b);

        printf("[VISUALIZZA] Valore=%d\n", valore);
    }

    return NULL;
}


void produci_valore(BufferProdCons * b, int valore) {

    /* TBD: Completare questa funzione introducendo la sincronizzazione */

    pthread_mutex_lock(&b->mutex);

    while(b->stato==PIENO){
        printf("[PRODUCI]: Non posso ancora produrre, buffer pieno\n");
        pthread_cond_wait(&b->VAR_PROD, &b->mutex);
   }

   printf("[PRODUCI]: Sono uscito dall'attesa\n");

    b->valore = valore;
    b->stato = PIENO;
   
   pthread_cond_signal(&b->VAR_CONS);

   pthread_mutex_unlock(&b->mutex);

}

int consuma_valore(BufferProdCons * b) {

    /* TBD: Completare questa funzione introducendo la sincronizzazione */

    pthread_mutex_lock(&b->mutex);

    while(b->stato==VUOTO){
        printf("[CONSUMA]: Non c'e' niente da consumare\n");
        pthread_cond_wait(&b->VAR_CONS, &b->mutex);
   }

   printf("[CONSUMA]: Sono uscito dall'attesa\n");

    int valore = b->valore;

    b->stato = VUOTO;

    pthread_cond_signal(&b->VAR_PROD);

    pthread_mutex_unlock(&b->mutex);

    return valore;
}
