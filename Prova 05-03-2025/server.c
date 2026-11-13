#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <pthread.h>

#include "header_msg.h"
#include "header_prodcons.h"

struct parametri_thread_1 {

    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
            includendo il buffer b1, e gli identificativi delle code di messaggi
    */
    int id_requesttosend;
    int id_oktosend;
    int id_mainqueue;
    struct buffer* b1;
};

struct parametri_thread_2 {
    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
            includendo il buffer b1 e il buffer b2
    */
    int id2;
    struct buffer* b1;
    struct buffer* b2;
};

struct parametri_thread_3 {
    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
            includendo il buffer b2
    */
    int id;
    struct buffer* b2;
};

void * thread_1(void *);
void * thread_2(void *);
void * thread_3(void *);

int main() {

    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t chiave_ots= ftok(".", 'f');
    key_t chiave_rts= ftok(".", 'g');
    key_t chiave_main= ftok(".", 'h');
    int id_msg_ots= msgget(chiave_ots, 0);
    int id_msg_rts= msgget(chiave_rts, 0);
    int id_msg_main= msgget(chiave_main, 0);


    struct buffer * b1 = /* TBD: Inizializzare il puntatore alla struttura dati */malloc(sizeof(struct buffer)); 
    struct buffer * b2 = /* TBD: Inizializzare il puntatore alla struttura dati */malloc(sizeof(struct buffer)); 

    buffer_init(b1);
    buffer_init(b2);


    struct parametri_thread_1 * p1 = /* TBD: Inizializzare il puntatore alla struttura dati */malloc(sizeof(struct parametri_thread_1));

    /* TBD: Inizializzare i valori nella struttura "p1" */
    p1->b1= b1;
    p1->id_mainqueue= id_msg_main;
    p1->id_oktosend= id_msg_ots;
    p1->id_requesttosend= id_msg_rts;

    //********PERCHE' FARE LA MALLOC ANCHE DEI PARAMATRI E NON SOLO DELLE STRUTTURE CONDIVISE? ***********

    struct parametri_thread_2 * p2 = /* TBD: Inizializzare il puntatore alla struttura dati */malloc(sizeof(struct parametri_thread_2));

    /* TBD: Inizializzare i valori nella struttura "p2" */
    p2->b1= b1;
    p2->b2= b2;


    struct parametri_thread_3 * p3 = /* TBD: Inizializzare il puntatore alla struttura dati */malloc(sizeof(struct parametri_thread_3));

    /* TBD: Inizializzare i valori nella struttura "p3" */
    p3->b2= b2;

    /* TBD: Creare i 3 thread, passandogli le 3 strutture dati con i parametri */
    pthread_t id_thread[3];
    
    printf("[SERVER]: Creazione dle Thread 1...\n");
    int err= pthread_create(&id_thread[0], NULL, thread_1, (void *)p1);

    if(err){
        perror("[WARNING_SERVER]: Errore nella creazione del Thread 1\n ");
        exit(1);
    }

    printf("[SERVER]: Creazione dle Thread 2...\n");
    int err2= pthread_create(&id_thread[1], NULL, thread_2, (void *)p2);

    if(err2){
        perror("[WARNING_SERVER]: Errore nella creazione del Thread 2\n ");
        exit(1);
    }


    printf("[SERVER]: Creazione dle Thread 3...\n");
    int err3= pthread_create(&id_thread[2], NULL, thread_3, (void *)p3);

    if(err3){
        perror("[WARNING_SERVER]: Errore nella creazione del Thread 3\n ");
        exit(1);
    }


    /* TBD: Attendere la terminazione dei 3 thread */
    for(int k=0; k<3; k++){
        
        pthread_join(id_thread[k], NULL);
        printf("[SERVER]: Terminazione del Thread n. %d\n", k+1);
    }

    buffer_destroy(b1);
    buffer_destroy(b2);

    free(b1);
    free(b2);
    free(p1);
    free(p2);
    free(p3);

    pthread_exit(0); //Perche' è necessario fare la pthread_exit anche nel main?


}


void * thread_1(void * x) {

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_1 *par= (struct parametri_thread_1*) x;

    struct rts rts;
    struct ots ots;
    struct richiesta r;

    int idrequest= par->id_requesttosend;
    int idoktosend= par->id_oktosend;
    int idmain= par->id_mainqueue;


    for(int i=0; i<3; i++) {

        /* TBD: Ricevere il messaggio "request to send" */
        int result= msgrcv(idrequest, (void *)&rts, sizeof(struct rts)-sizeof(long), REQUEST_TO_SEND, 0);

        if(result<0){
            printf("[WARNING_THREAD 1]: Errore nella ricezione del messaggio REQUEST_TO_SEND\n");
            exit(1);

        }else{
        printf("[THREAD 1] Ricevuto messaggio RTS\n");
        }


        /* TBD: Inviare il messaggio "ok to send" */
        ots.tipo_ok= OK_TO_SEND;
        ots.mess_ok= 0;

        int result2= msgsnd(idoktosend, (void*)&ots, sizeof(struct ots)-sizeof(long), 0);

        if(result<0){
            printf("[WARNING_THREAD 1]: Errore nell'invio del messaggio OK_TO_SEND\n");
            exit(1);

        }else{

        printf("[THREAD 1] Inviato messaggio OTS\n");
        }


        /* TBD: Ricevere il messaggio con il valore */
        int result3= msgrcv(idmain, (void *)&r, sizeof(struct richiesta)-sizeof(long), MESSAGGIO, 0);

        if(result<0){
            printf("[WARNING_THREAD 1]: Errore nella ricezione del messaggio \n");
            exit(1);

        }

        int valore = /* TBD */r.mess;

        printf("[THREAD 1] Ricevuto messaggio MSG con valore %d\n", valore);


        buffer_produci(/* TBD */par->b1, valore);

        printf("[THREAD 1] Inserito valore %d nel buffer 1\n", valore);
    }

    pthread_exit(NULL);
    
}

void * thread_2(void * x) {

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_2 *par= (struct parametri_thread_2*) x;

    int valore;

    for(int i=0; i<3; i++) {

        valore = buffer_consuma(/* TBD */par->b1);


        printf("[THREAD 2] Prelevato valore %d dal buffer 1\n", valore);


        sleep(2);


        valore = valore * 2;

        buffer_produci(/* TBD */par->b2, valore);

        printf("[THREAD 2] Inserito valore %d nel buffer 2\n", valore);
    }

    pthread_exit(NULL);
}

void * thread_3(void * x) {

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_3 *par= (struct parametri_thread_3*) x;

    int valore;

    for(int i=0; i<3; i++) {

        valore = buffer_consuma(/* TBD */par->b2);

        printf("[THREAD 3] Prelevato valore %d dal buffer 2\n", valore);

        sleep(1);

        valore = valore + 1;

        printf("[THREAD 3] Valore finale: %d\n", valore);

    }

    pthread_exit(NULL);
}

