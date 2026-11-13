#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <unistd.h>
#include <sys/shm.h>

#include "header.h"

int main() {

    /* TBD: Creare le code di messaggi */
    key_t key_coda1= ftok(".", 'f');
    key_t key_coda2= ftok(".", 'g');
 
    int idmess_req= msgget(key_coda1, IPC_CREAT|0664);
    int idmess_risp= msgget(key_coda2, IPC_CREAT|0664);


    if(idmess_req<0){

        printf("[WARNING_START]: Errore nella creazione della coda di messaggi\n");
        exit(1);
    }

    if(idmess_risp<0){

        printf("[WARNING_START]: Errore nella creazione della coda di messaggi\n");
        exit(1);
    }



    for(int i=0; i<PROCESSI_CLIENT; i++) {
        
        /* TBD: Creare i processi client */

        pid_t pid = fork();
            if(pid==0){

                printf("[START]: Creo il processo Client n. %d\n", i+1);
                if(execl("./client", "client", NULL)<0){
                    printf("[WARNING_START]: Errore nella creazione del processo Client\n");
                    exit(1);
                };
            }else if (pid<0){
                printf("[WARNING_START]: Errore nella creazione tramite pid del processo Client\n");
                exit(1);
            }

    }


    for(int i=0; i<PROCESSI_SERVER; i++) {
        
        /* TBD: Creare i processi server */
        pid_t pid= fork();
            if(pid==0){
                printf("[START]: Creazione processo Server n. %d\n", i+1);
                if(execl("./server", "server", NULL)<0){
                    printf("[WARNING_START]: Errore nella creazione di un processo Server\n");
                    exit(1);
                }
            }else if (pid<0){
                printf("[WARNING_START]: Errore nella creazione del procesos Server tramite pid\n");
                exit(1);
            }

    }


    /* TBD: Creare il processo database */
    pid_t pid= fork();
    if(pid==0){
        printf("[START]: Creazione del processo Database\n");
        if(execl("./database", "database", NULL)<0){
            printf("[WARNING_START]: Errore nella creazione del processo Database\n");
            exit(1);
        }

    }else if(pid<0){
        printf("[WARNING_START]: Errore nella creazione dle processo Database tramite pid\n");
        exit(1);
    }


    /* TBD: Attendere e gestire la terminazione */
    for(int k=0; k<PROCESSI_CLIENT; k++){
        printf("[START]: Terminazione del processo Client n. %d\n", k+1);
        wait(NULL);
    }


    for(int k=0; k<PROCESSI_SERVER; k++){
        printf("[START]: Terminazione del processo Server n. %d\n", k+1);
        wait(NULL);
    }

    
        printf("[START]: Terminazione del processo Database\n");
        wait(NULL);

    msgctl(key_coda1, IPC_RMID, 0);
    msgctl(key_coda2, IPC_RMID, 0);
    

}