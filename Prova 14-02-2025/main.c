#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "header_msg.h"

int main() {

    key_t chiave_collettore = /*TODO*/ftok(".", 'a');
    int id_queue_collettore = /*TODO*/msgget(chiave_collettore, IPC_CREAT|0664);

    if(id_queue_collettore<0){
        printf("[MAIN]: Errore nella creazione della coda collettore\n");
        exit(1);
    }

    key_t chiave_server = /*TODO*/ftok(".", 'n');
    int id_queue_server = /*TODO*/msgget(chiave_server, IPC_CREAT|0664);

    if(id_queue_collettore<0){
        printf("[MAIN]: Errore nella creazione della coda server\n");
        exit(1);
    }
    
    //TODO: creazione ed eseuzione processo server
    pid_t pid= fork();

    if(pid==0){
        
        printf("[MAIN]: Creo il processo Server...  \n");

        if(execl("./server", "server", NULL)<0){
            
            printf("[MAIN]: Errore nella creazione del nuovo processo server\n");
            exit(1);

        }
    }else if(pid<0){

        printf("[MAIN]: Errore nella creazione del processo figlio Server\n");
        exit(1);
    }


    //TODO: creazione processo collettore
    pid_t pid_coll= fork();
    if(pid_coll==0){

        collettore(id_queue_collettore, id_queue_server);
        exit(0);
    }else if(pid_coll<0){

        printf("[MAIN]: Errore nella creazione del processo figlio Collettore\n");
        exit(1);
    }

    //TODO: creazione processi sensori, ciascuno con il proprio ID
    for(int i=0; i<NUM_SENSORI; i++){

        pid_t pid=fork();

            if(pid==0){

                int identificativo= i+1;
                printf("[MAIN]: Creazione del processo Sensore con pid %d\n", identificativo);
                sensore(identificativo, id_queue_collettore);
                exit(0);
            }else if(pid<0){

                printf("[MAIN]: Problema nella creazione di un processo consumatore\n");
                exit(1);
            }
    }
    

    //TODO: attesa dei processi figli
    for(int k=0; k<NUM_SENSORI+1; k++){
    wait(NULL);
    printf("[MAIN]: Processi figli terminati.\n");    
    }
    
    //TODO: rimozione risorse IPC
    msgctl(id_queue_collettore, IPC_RMID, 0);
    msgctl(id_queue_server, IPC_RMID, 0);
    
    return 0;
}