#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "pubsub.h"

void publisher(int id_coda_messaggi, long topic);

int main() {


    key_t chiave_coda_messaggi = ftok("./", 'g');/* TBD */

    int id_coda_messaggi = msgget(chiave_coda_messaggi, 0);/* TBD: Creare una coda di messaggi dei publisher */






    for(int i=0; i<NUM_PUBLISHERS_PER_TOPIC; i++) {

        printf("[PUBLISHER 1] Avvio processo publisher su topic 1\n");

        /* TBD: 
            Creare un processo figlio, che esegua la funzione "publisher".
            Passare come parametri lo ID della coda messaggi, e il valore TOPIC1.
        */
        pid_t pid=fork();

        if(pid==0){
            publisher(id_coda_messaggi, TOPIC1);
            exit(0);
        }else if(pid<0){
            printf("[PUBLISHER 1]: Errore con il pid n. %d\n", i+1);
            exit(1);
        }

    }


    for(int i=0; i<NUM_PUBLISHERS_PER_TOPIC; i++) {

        printf("[PUBLISHER 2] Avvio processo publisher su topic 2\n");

        /* TBD: 
            Creare un processo figlio, che esegua la funzione "publisher".
            Passare come parametri lo ID della coda messaggi, e il valore TOPIC2.
        */

        pid_t pid=fork();

        if(pid==0){
            publisher(id_coda_messaggi, TOPIC2);
            exit(0);
        }else if(pid<0){
            printf("[PUBLISHER 2]: Errore con il pid n. %d\n", i+1);
            exit(1);
        }
    }


    printf("[PUBLISHER] Attesa terminazione processi figli\n");

    /* TBD: Attendere la terminazione dei processi figli */
    for(int k=0; k<NUM_PUBLISHERS_PER_TOPIC*2; k++){
        wait(NULL);

        printf("[PUBLISHER]: Processo publisher n. %d terminato\n", k+1);
    }


}

void publisher(int id_coda_messaggi, long topic) {

    srand(getpid());

    for(int i=0; i<NUM_MESSAGGI; i++) {

        int valore = rand() % 100;

        messaggio_valore messaggio;

        /* TBD:
            Inviare un messaggio al processo broker.
            Includere il topic ed il valore.
        */

        messaggio.topic=topic;
        messaggio.valore= valore;

        int result= msgsnd(id_coda_messaggi, (void *)&messaggio, sizeof(messaggio)-sizeof(long), 0);

        if(result<0){perror("[PUBLISHER_FUNCTION]: Errore nell'invio di un messaggio alla coda dei messaggi\n"); exit(1);}
        

        printf("[PUBLISHER] Messaggio inviato: topic=%ld, valore=%d\n", topic, valore);

        sleep(1);
    }
}

