#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include "header.h"

#define NUM_VIEWERS 3
#define NUM_UPDATERS 2


int main(){

    pid_t pid;
    int status, k;
    
    //Creazione code richieste e risposte
    int ds_queue_request, ds_queue_response;

    //create queues
    int key_queue_request = ftok("./", 'f');// TODO aggiungere codice per la generazione della chiave
    int key_queue_response = ftok("./", 'g');// TODO aggiungere codice per la generazione della chiave
    
    // TODO aggiungere codice di inizializzazione delle code
    ds_queue_request = msgget(key_queue_request, IPC_CREAT|0664);//...
    ds_queue_response = msgget(key_queue_response, IPC_CREAT|0664);//...
    
    printf("[master] Coda ds_queue_request creata! ID: %d\n", ds_queue_request);
    printf("[master] Coda ds_queue_response creata! ID: %d\n", ds_queue_response);
    
    // TODO aggiungere codice per generare NUM_VIEWERS processi viewers, attraverso l'exec
    for(int j=0; j<NUM_VIEWERS; j++){

        pid=fork();
            if(pid==0){
                if(execl("./viewer", "viewer", NULL)<0){perror("[MAIN]: Errore nella exec del Viewer\n"); exit(1);}
            }else if(pid<0){
                printf("[MAIN]: Errore nel pid del viewer\n");
                exit(1);
            }
    }

    // TODO aggiungere codice per generare NUM_UPDATERS processi updaters, attraverso l'exec

    for(int j=0; j<NUM_UPDATERS; j++){

        pid=fork();
            if(pid==0){
                if(execl("./updater", "updater", NULL)<0){perror("[MAIN]: Errore nella exec dell'Updater\n"); exit(1);}
            }else if(pid<0){
                printf("[MAIN]: Errore nel pid dell'updater\n");
                exit(1);
            }
    }
    

    // TODO aggiungere codice per generare 1 processo store, attraverso l'exec
     pid=fork();
        if(pid==0){
            if(execl("./store", "store", NULL)<0){perror("[MAIN]: Errore nella exec dello store\n"); exit(1);}
        }else if(pid<0){
            printf("[MAIN]: Errore nel pid dello store\n");
            exit(1);
        }
    

    for (k=0; k<NUM_VIEWERS+NUM_UPDATERS+1; k++){
        pid = wait(&status);
        if (pid == -1)
            perror("errore");
        else
            printf ("[master] Figlio n.ro %d e\' morto con status= %d \n", pid, status);
    }

    // TODO aggiungere codice per la rimozione delle code
    msgctl(ds_queue_request, IPC_RMID, NULL);
    msgctl(ds_queue_response, IPC_RMID, NULL);

    printf("[master] code rimosse\n");
    
    return 0;
}
