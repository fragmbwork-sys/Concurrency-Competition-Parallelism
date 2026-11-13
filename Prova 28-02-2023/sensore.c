#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_msg.h"

void sensore(int id_sensore, int id_queue_collettore) {

    srand(getpid());

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        messaggio_sensore m;

        int valore = (rand() % 10) + 10;

        m.valore= valore;
        m.id= id_sensore;

        /* TBD: Inviare "valore" mediante messaggio */
        int result= msgsnd(id_queue_collettore, (void *)&m, sizeof(messaggio_sensore)-sizeof(long), 0);

        if(result<0){perror("[SENSORE]: Errore nell'invio di un messaggio al collettore\n"); exit(1);}

        printf("[SENSORE] Inviato valore=%d\n", valore);


        sleep(3);
    }
}