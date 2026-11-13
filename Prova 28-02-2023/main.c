#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "header_msg.h"

int main() {

    /* TBD: Creare le code di messaggi */
    key_t key_msg1= ftok("./", 'f');
    key_t key_msg2= ftok("./", 'g');
    key_t key_msg_Server= ftok("./", 'h');


    int id_queue_collettore_1 = /* TBD */msgget(key_msg1, IPC_CREAT|0664);


    int id_queue_collettore_2 = /* TBD */msgget(key_msg2, IPC_CREAT|0664);


    int id_queue_server = /* TBD */msgget(key_msg_Server, IPC_CREAT|0664);


    if(id_queue_collettore_1<0){
        printf("[MAIN]: Errore nella creazione della coda di messaggi collettore 1\n");
        exit(1);
    }

    if(id_queue_collettore_2<0){
        printf("[MAIN]: Errore nella creazione della coda di messaggi collettore 2\n");
        exit(1);
    }

    if(id_queue_server<0){
        printf("[MAIN]: Errore nella creazione della coda di messaggi server\n");
        exit(1);
    }




    /* TBD: Creare il processo figlio Server, 
            passandogli lo id della coda */
        pid_t pid= fork();

        if(pid==0){
                printf("[MAIN]: Creazione del processo Server\n");
                 server(id_queue_server);
                 exit(0);
        }else if(pid<0){
                printf("[MAIN]: Errore nella creazione del pid\n");
                exit(1);
        }



    int id_collettore = 1;

    /* TBD: Creare il primo processo figlio Collettore, 
            passandogli lo "id_collettore",
            lo id della coda per ricevere dai sensori,
            e lo id della coda per inviare al server
    */
    pid_t pid2= fork();

    if(pid2==0){
        printf("[MAIN]: Creazione del PRIMO processo Collettore\n");
        collettore(id_collettore, id_queue_collettore_1, id_queue_server);
        exit(0);
    }



    id_collettore = 2;
    
    /* TBD: Creare il secondo processo figlio Collettore, 
            passandogli lo "id_collettore",
            lo id della coda per ricevere dai sensori,
            e lo id della coda per inviare al server
    */

    pid_t pid3= fork();

    if(pid3==0){
        printf("[MAIN]: Creazione del SECONDO processo Collettore\n");
        collettore(id_collettore, id_queue_collettore_2, id_queue_server);
        exit(0);
    }


    for(int i=0; i<NUM_SENSORI_PER_COLLETTORE; i++) {

        int id_sensore = i+1;

        /* TBD: Creare i processi figli Sensore, 
                passandogli lo "id_sensore",
                e lo id della coda per inviare al primo collettore
        */
        pid_t pid=fork();

        if(pid==0){
                printf("[MAIN]: Creazione del processo Sensore n. %d\n", i+1);
                sensore(id_sensore, id_queue_collettore_1);
                exit(0);
        }

    }


    for(int i=0; i<NUM_SENSORI_PER_COLLETTORE; i++) {

        int id_sensore = i+1;

        /* TBD: Creare i processi figli Sensore, 
                passandogli lo "id_sensore",
                e lo id della coda per inviare al secondo collettore
        */
        pid_t pid=fork();

        if(pid==0){
                printf("[MAIN]: Creazione del processo Sensore n. %d\n", i+1);
                sensore(id_sensore, id_queue_collettore_2);
                exit(0);
        }
    }



    /* TBD: Attendere la terminazione di tutti i processi figli */
    for(int k=0; k<NUM_COLLETTORI; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del proceso collettore n. %d\n", k+1);
    }

    for(int l=0; l<NUM_SENSORI_PER_COLLETTORE*NUM_COLLETTORI; l++){
        wait(NULL);
        printf("[MAIN]: Terminazione del proceso Sensore n. %d\n", l+1);
    }


    /* TBD: De-allocare le code di messaggi */
    msgctl(id_queue_server, IPC_RMID, NULL);
    msgctl(id_queue_collettore_1, IPC_RMID, NULL);
    msgctl(id_queue_collettore_2, IPC_RMID, NULL);

    
    return 0;
}