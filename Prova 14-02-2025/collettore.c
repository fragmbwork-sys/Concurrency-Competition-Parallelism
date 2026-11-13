#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_msg.h"


void collettore(int id_queue_collettore, int id_queue_server) {

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int somma_valori = 0;
        int media = 0;

        for(int j=0; j<NUM_SENSORI; j++) {

            //TODO: riceve un messaggio da ogni sensore
            int ID= j+1;
            messaggio_sensore mess;
            msgrcv(id_queue_collettore, (void*)&mess, sizeof(messaggio_sensore)-sizeof(long), ID, 0);


            int valore = mess.messaggio;//TODO;

            printf("[COLLETTORE] Ricevuto valore=%d\n", valore);

            somma_valori += valore;
        }


        media = somma_valori/3; //TODO: calcolo media

        //TODO manda la media al server
        messaggio_collettore mess2;
        mess2.media= media;
        mess2.tipo_coll= TIPO_COLLETTORE;

        int result= msgsnd(id_queue_server, (void*)&mess2, sizeof(messaggio_collettore)-sizeof(long), 0);
        
        if(result<0){perror("[COLETTORE]: Errore nell'invio di un messaggio\n");}

        printf("[COLLETTORE] Inviato valore=%d\n", media);


    }
}
