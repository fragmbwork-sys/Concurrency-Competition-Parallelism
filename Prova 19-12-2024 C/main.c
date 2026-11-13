#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"



int main() {

    /*
        TODO: creazione code di messaggi
    */
    key_t chiave_coda_richieste= ftok(".", 'f');
    key_t chiave_coda_risposte= ftok(".", 'g');

    int id_msg_richieste= msgget(chiave_coda_richieste, IPC_CREAT|0664);
    int id_msg_risposte= msgget(chiave_coda_risposte, IPC_CREAT|0664);


    if (id_msg_richieste == -1 || id_msg_risposte == -1) {
        perror("Errore nella creazione delle code di messaggi");
        exit(1);
    }

    /*
        TODO: avvio client e server in eseguibili distinti
    */
    pid_t pid= fork();
        if(pid== 0){

            printf("[MAIN]: Creazione del processo Server...\n");
            if(execl("./server", "server", NULL)<0){
                printf("[WARNING_MAIN]: Errore nella creazione del processo server\n");
                exit(1);
            }
        }else if(pid<0){
            printf("[WARNING_MAIN]: Il pid durante la creazione del Server ha fatto capricci...\n");
            exit(1);
        }
    
    pid_t pid2= fork();
    if(pid2==0){
        printf("[MAIN]: Creazione del processo Client...\n");
        if(execl("./client", "client", NULL)<0){
            printf("[WARNING_MAIN]: Errore nella creazione del processo Client\n");
            exit(1);
        }
    }else if(pid<0){
            printf("[WARNING_MAIN]: Il pid durante la creazione del Client ha fatto capricci...\n");
            exit(1);
        }

    /*
        TODO: attendo terminazione figli
    */
    for(int k=0; k<2; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);

    }

    msgctl(id_msg_richieste, IPC_RMID, NULL);
    msgctl(id_msg_risposte, IPC_RMID, NULL);

    return 0;
}
