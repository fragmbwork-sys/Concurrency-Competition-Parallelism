#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"

int main() {
    /*
        TODO: creazione code di messaggi 
    */
    int key_richieste= ftok("./", 'a');
    int coda_richieste= msgget(key_richieste, IPC_CREAT|0664);

    int key_risposte= ftok("./", 'z');
    int coda_risposte= msgget(key_risposte, IPC_CREAT|0664);


    if (coda_richieste == -1 || coda_risposte == -1) {
        perror("Errore nella creazione delle code");
        exit(1);
    }


    /*
        TODO: avvio del client e server in eseguibili distinti
    */

    pid_t pid= fork();

        if(pid==0){

            printf("[MAIN]: Creazione del processo Client...\n");
            if(execl("./client", "client", NULL)<0){
                printf("[WARNING_MAIN]: Errore nella creazione del processo Client\n");
                exit(1);
            }
        }else if(pid<0){

            printf("[WARNING_MAIN]: Errore nella creazione del processo C\n");
            exit(1);
        }

    pid_t pid2= fork();

        if(pid2==0){

            printf("[MAIN]: Creazione del processo Server...\n");
            if(execl("./server", "server", NULL)<0){
                printf("[WARNING_MAIN]: Errore nella creazione del processe Server\n");
                exit(1);
            }
        }else if(pid2<0){

            printf("[WARNING_MAIN]: Errore nella creazione del processo S\n");
            exit(1);
        }

    

    /*
        TODO: attendere terminazione processi
    */
    //while(wait(NULL)>0);

  for(int k=0; k<2; k++){
    wait(NULL);
    printf("[MAIN]: Il processo %d e' terminato.\n", k+1);
   }

    msgctl(coda_richieste, IPC_RMID, NULL);
    msgctl(coda_risposte, IPC_RMID, NULL);

    return 0;
}