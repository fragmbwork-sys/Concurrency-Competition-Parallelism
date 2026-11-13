#include <stdio.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <pthread.h>

#include "pubsub.h"

#define MAX_TOPICS 2
#define MAX_SUBSCRIBERS 2

int code[MAX_TOPICS][MAX_SUBSCRIBERS];

void * gestisci_registrazioni(void * arg);
void * gestisci_messaggi(void * arg);


int main() {

    // inizializzazione lista di code
    for(int i=0; i<MAX_TOPICS; i++) {
        for(int j=0; j<MAX_SUBSCRIBERS; j++) {
            code[i][j] = 0;
        }
    }


    key_t chiave_coda_registrazioni = /* TBD */ftok("./", 'f');

    int id_coda_registrazioni = msgget(chiave_coda_registrazioni, IPC_CREAT|0664);/* TBD: Creare una coda di messaggi di registrazione */



    key_t chiave_coda_messaggi = ftok("./", 'g');/* TBD */

    int id_coda_messaggi = msgget(chiave_coda_messaggi, IPC_CREAT|0664);/* TBD: Creare una coda di messaggi dei publisher */


    

    
    /* TBD:
        Creare un thread che esegua la funzione "gestisci_registrazioni".
        Passare come parametro alla funzione gli id delle code.
     */
    pthread_t id_thrd_registrazioni;
    int err= pthread_create(&id_thrd_registrazioni, NULL, gestisci_registrazioni, (void*)&id_coda_registrazioni);

    if(err){perror("[BROKER]: Errore nella creazione del thread Registrazioni\n"); exit(1);}

    
    
    /* TBD:
        Creare un thread che esegua la funzione "gestisci_messaggi".
        Passare come parametro alla funzione gli id delle code.
     */
    pthread_t id_thrd_messaggi;
    int err2= pthread_create(&id_thrd_messaggi, NULL, gestisci_messaggi, (void*)&id_coda_messaggi);

    if(err2){perror("[BROKER]: Errore nella creazione del thread Messaggi\n"); exit(1);}

    /* TBD: Attendere la terminazione dei 2 thread */

    
        pthread_join(id_thrd_registrazioni, NULL);
        printf("[BROKER]: Thread Registrazioni terminato.\n");

        pthread_join(id_thrd_messaggi, NULL);
        printf("[BROKER]: Thread Messaggi terminato.\n");
    

    /* TBD: Deallocare le 2 code di messaggi */
    msgctl(id_coda_registrazioni, IPC_RMID, NULL);
    msgctl(id_coda_messaggi, IPC_RMID, NULL);

}


void * gestisci_registrazioni(void * arg) {


    /* TBD: Completare il passaggio di parametri */
    int * coda_registrazioni= (int*) arg;

    for(int i=0; i<TOTALE_SUBSCRIBER; i++) {

        messaggio_registrazione messaggio;

        /* TBD: Ricevere un messaggio di registrazione */
        int result= msgrcv(*coda_registrazioni, (void*)&messaggio, sizeof(messaggio_registrazione)-sizeof(long), 0, 0);

        if(result<0){perror("[REGISTRAZIONI]: Errore nella ricezione di un messaggio\n"); exit(1);}

        long topic = /* TBD */messaggio.topic;
        int id_coda = /* TBD */messaggio.ID_CODA;

        printf("[BROKER]: Ricevuto messaggio di registrazione: topic=%ld, id_coda=%d\n", topic, id_coda);


        // aggiungo il subscriber alla lista di code

        if(topic <= 0 || topic > MAX_TOPICS) {
            printf("[BROKER]: Topic non valido\n");
            exit(1);
        }

        if(id_coda <= 0) {
            printf("[BROKER]: Id coda non valido\n");
            exit(1);
        }

        int trovato = 0;
        for(int i=0; i<MAX_SUBSCRIBERS; i++) {
            if(code[topic-1][i] == 0) {
                code[topic-1][i] = id_coda;
                trovato = 1;
                break;
            }
        }

        if(trovato == 0) {
            printf("[BROKER]: Numero massimo di subscriber raggiunto\n");
            exit(1);
        }

    }

    return NULL;
}


void * gestisci_messaggi(void * arg) {


    /* TBD: Completare il passaggio di parametri */
    int * coda_messaggi= (int*) arg;

    for(int i=0; i<TOTALE_MESSAGGI; i++) {

        messaggio_valore messaggio;

        /* TBD: Ricevere un messaggio dai publisher  */
    int result= msgrcv(*coda_messaggi, (void *)&messaggio, sizeof(messaggio_valore)-sizeof(long), 0, 0);

    if(result<0){perror("[MESSAGGI]: Errore nella ricezione di un messaggio\n"); exit(1);}

        long topic = /* TBD */messaggio.topic;
        int valore = /* TBD */messaggio.valore;

        printf("[BROKER]: Ricevuto messaggio: topic=%ld, valore=%d\n", topic, valore);



        // invio messaggio a tutti i subscriber registrati al topic

        if(topic <= 0 || topic > MAX_TOPICS) {
            printf("[BROKER]: Topic non valido\n");
            exit(1);
        }

        for(int j=0; j<MAX_SUBSCRIBERS; j++) {

            int id_coda = code[topic-1][j];

            if(id_coda != 0) {

                /* TBD: Inviare il messaggio al subscriber in ascolto sulla coda "id_coda" */
                messaggio_valore mess;
                mess.topic= topic;
                mess.valore= valore;

                int result= msgsnd(id_coda, (void *)&mess, sizeof(messaggio_valore)-sizeof(long), 0);

                if(result<0){perror("[MESSAGGI]: Errore nell'invio del messaggio ai Subscriber\n"); exit(1);}

            }
        }

    }

    return NULL;
}
