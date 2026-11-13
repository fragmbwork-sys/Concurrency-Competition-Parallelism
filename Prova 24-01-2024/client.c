#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header_msg.h"
#include "header_sem.h"


int main() {

    srand(getpid());


    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t chiave_coda_mess= ftok("./", 'f');

    int id_coda_mess= msgget(chiave_coda_mess, 0);

    if(id_coda_mess<0){
        printf("[CLIENT]: Errore nella creazione della coda dei messaggi\n");
        exit(1);
    }




    for(int i=0; i<NUM_RICHIESTE; i++) {

        int buffer = rand() % NUM_BUFFER;

        int value = rand() % 10;


        /* TBD: Inviare un messaggio contenente "value" e "buffer".

                È richiesto che l'invio sia effettuato
                tramite send sincrona.
         */
         MessaggioClient mess;
         mess.tipo= REQUEST_TO_SEND;

         int result= msgsnd(id_coda_mess, (void *)&mess, sizeof(MessaggioClient)-sizeof(long), 0);

         if(result<0){perror("[CLIENT]: Errore nell'invio del messaggio di RQST_TO_SND al server\n"); exit(1); }

         MessaggioClient risposta;

         result= msgrcv(id_coda_mess, (void *)&risposta, sizeof(MessaggioClient)-sizeof(long), OK_TO_SEND, 0);

         if(result<0){perror("[CLIENT]: Errore nella ricezione del messaggio OK_TO_SND dal server\n"); exit(1); }

         MessaggioClient m;
         m.tipo= TIPO_MESS;
         m.valore= value;
         m.buffer_destinazione= buffer;

         result= msgsnd(id_coda_mess, (void *)&m, sizeof(MessaggioClient)-sizeof(long), 0);

         if(result<0){perror("[CLIENT]: Errore nell'invio del messaggio al server\n"); exit(1); }

         printf("[CLIENT]: Messaggio Inviato!\n");


        sleep(2);

    }




}