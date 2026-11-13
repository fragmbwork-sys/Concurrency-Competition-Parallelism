#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include <wait.h>

#include "header_msg.h"

int main() {

    /* TBD: Creare le code di messaggi per la send sincrona*/
    key_t chiave_ots= ftok(".", 'f');
    key_t chiave_rts= ftok(".", 'g');
    key_t chiave_main= ftok(".", 'h');
    int id_msg_ots= msgget(chiave_ots, IPC_CREAT|0664);
    int id_msg_rts= msgget(chiave_rts, IPC_CREAT|0664);
    int id_msg_main= msgget(chiave_main, IPC_CREAT|0664);

    if(id_msg_ots<0){
        printf("[WARNING_START]: Errore nella creazione della coda OK TO SEND\n");
        exit(1);
    }

    if(id_msg_rts<0){
        printf("[WARNING_START]: Errore nella creazione della coda REQUEST TO SEND\n");
        exit(1);
    }

    if(id_msg_main<0){
        printf("[WARNING_START]: Errore nella creazione della coda MAIN\n");
        exit(1);
    }


    /* TBD: Creare un processo figlio, ed eseguire il programma "server" */
    pid_t pid= fork();

    if(pid==0){
        printf("[START]: Creazione del processo SERVER...\n");
        if(execl("./server", "server", NULL)<0){
            printf("[WARNING_START]: Errore nella creazione del processo SERVER\n");
            exit(1);
        }
    }



    /* TBD: Creare un processo figlio, ed eseguire il programma "client" */

    pid_t pid2= fork();

    if(pid2==0){
        printf("[START]: Creazione del processo CLIENT...\n");
        if(execl("./client", "client", NULL)<0){
            printf("[WARNING_START]: Errore nella creazione del processo CLIENT\n");
            exit(1);
        }
    }

    /* TBD: Attendere la terminazione dei processi figli */
    for(int k=0; k<2; k++){
        
        wait(NULL);
        printf("[START]: Terminazione del processo n. %d\n", k+1);
    }


    /* TBD: Eliminare le code di messaggi */
    shmctl(id_msg_rts, IPC_RMID, NULL);
    shmctl(id_msg_ots, IPC_RMID, NULL);
    shmctl(id_msg_main, IPC_RMID, NULL);

}