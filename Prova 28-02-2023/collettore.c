#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_msg.h"


void collettore(int id_collettore, int id_queue_collettore, int id_queue_server) {

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int somma_valori = 0;
        int media = 0;

        for(int j=0; j<NUM_SENSORI_PER_COLLETTORE; j++) {

            int id_sensore = j+1;

            messaggio_sensore m;

            /* TBD: Ricevere un messaggio da un sensore, con ricezione selettiva */
            int result= msgrcv(id_queue_collettore, (void *)&m, sizeof(messaggio_sensore)-sizeof(long), id_sensore, 0);

            if(result<0){perror("[COLLETTORE]: Errore nella ricezione di un messaggio\n"); exit(1);}

            int valore = /* TBD */m.valore;

            printf("[COLLETTORE] Ricevuto valore=%d\n", valore);

            somma_valori += valore;
        }


        media = somma_valori / NUM_SENSORI_PER_COLLETTORE;

        messaggio_collettore m;

        /* TBD: Inviare "media" al processo server */
        m.valore_C= media;
        m.id_C= id_collettore;
        
        int result= msgsnd(id_queue_server, (void *)&m, sizeof(messaggio_collettore)-sizeof(long), 0);

        if(result<0){perror("[COLLETTORE]: Errore nell'invio di un messaggio verso la coda server\n"); exit(1);}

        printf("[COLLETTORE] Inviato valore=%d\n", media);


    }
}
