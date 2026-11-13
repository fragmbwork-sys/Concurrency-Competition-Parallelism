#include <stdio.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "pubsub.h"

void subscriber(int id_coda_registrazioni, long topic);

int main() {

    srand(getpid());



    
    key_t chiave_coda_registrazioni = /* TBD */ftok("./", 'f');

    int id_coda_registrazioni = msgget(chiave_coda_registrazioni, 0);/* TBD: Creare una coda di messaggi di registrazione */




    for(int i=0; i<NUM_SUBSCRIBERS_PER_TOPIC; i++) {

        printf("[SUBSCRIBER 1] Avvio processo subscriber su topic 1\n");

        /* TBD: 
            Creare un processo figlio, che esegua la funzione "subscriber".
            Passare come parametri lo ID della coda per le registrazioni, e il valore TOPIC1.
        */
        pid_t pid=fork();

        if(pid==0){
            subscriber(id_coda_registrazioni, TOPIC1);
            exit(0);
        }else if(pid<0){
            printf("[SUBSCRIBER 1]: Errore con il pid n. %d\n", i+1);
            exit(1);
        }
    }


    for(int i=0; i<NUM_SUBSCRIBERS_PER_TOPIC; i++) {

        printf("[SUBSCRIBER 2] Avvio processo subscriber su topic 2\n");

        /* TBD: 
            Creare un processo figlio, che esegua la funzione "subscriber".
            Passare come parametri lo ID della coda per le registrazioni, e il valore TOPIC2.
        */
        pid_t pid= fork();

        if(pid==0){
            subscriber(id_coda_registrazioni, TOPIC2);
            exit(0);
        }else if(pid<0){
            printf("[SUBSCRIBER 2]: Errore con il pid n. %d\n", i+1);
            exit(1);
        }

    }


    printf("[SUBSCRIBER] Attesa terminazione processi figli\n");

    /* TBD: Attendere la terminazione dei processi figli */
    for(int k=0; k<NUM_SUBSCRIBERS_PER_TOPIC*2; k++){
        wait(NULL);

        printf("[SUBSCRIBER]: Processo subscriber n. %d terminato\n", k+1);
    }

}

void subscriber(int id_coda_registrazioni, long topic) {


    key_t chiave_coda_messaggi_subscriber = IPC_PRIVATE;/* TBD */

    int id_coda_messaggi_subscriber = msgget(chiave_coda_messaggi_subscriber, IPC_CREAT|0664);/* TBD: Creare una coda di messaggi privata del subscriber */




    messaggio_registrazione registrazione;

    /* TBD: 
        Inviare un messaggio di registrazione al broker.
        Includere nel messaggio lo ID della coda di messaggi privata.
    */
    registrazione.topic= topic;
    registrazione.ID_CODA= id_coda_messaggi_subscriber;

    int result= msgsnd(id_coda_registrazioni, (void *)&registrazione, sizeof(messaggio_registrazione)-sizeof(long), 0);

    if(result<0){perror("[SUBSCRIBER_FUNCTION]: Errore nell'invio di un messaggio nella coda delle registrazioni\n"); exit(1);}


    for(int i=0; i<NUM_MESSAGGI*NUM_PUBLISHERS_PER_TOPIC; i++) {

        messaggio_valore messaggio;

        /* TBD: Ricevere un messaggio dal broker */
        int result= msgrcv(id_coda_messaggi_subscriber, (void *)&messaggio, sizeof(messaggio_valore)-sizeof(long), topic, 0);

        int valore = /* TBD */messaggio.valore;

        printf("[SUBSCRIBER] Ricevuto messaggio: topic=%ld, valore=%d\n", topic, valore);

    }

    /* TBD: Deallocare la coda di messaggi privata del subscriber */
    msgctl(id_coda_messaggi_subscriber, IPC_RMID, NULL);


}

