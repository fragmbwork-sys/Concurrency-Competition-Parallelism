//#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header_msg.h"

int main() {


    /* TBD: Predisporre le code di messaggi UNIX per la send sincrona */
    key_t chiave_coda_mess= ftok("./", 'f');

    int id_coda_mess= msgget(chiave_coda_mess, IPC_CREAT|0664);

    if(id_coda_mess<0){
        printf("[MAIN]: Errore nella creazione della coda dei messaggi\n");
        exit(1);
    }




    /* TBD: Creare un processo figlio, che esegua l'eseguibile "server" */
    pid_t pid_s= fork();
    if(pid_s==0){

        if(execl("./server", "server", NULL)<0){perror("[MAIN]: Errore nella creazione del processo Server"); exit(1);}
    
    }else if(pid_s<0){
        printf("[MAIN]: Errore nella creazione tramite pid Server\n");
        exit(1);
    }


    /* TBD: Creare un processo figlio, che esegua l'eseguibile "client" */
    pid_t pid_c= fork();
    if(pid_c==0){

        if(execl("./client", "client", NULL)<0){perror("[MAIN]: Errore nella creazione del processo Client"); exit(1);}
    
    }else if(pid_c<0){
        printf("[MAIN]: Errore nella creazione tramite pid Client\n");
        exit(1);
    }


    /* TBD: Attendere la terminazione dei figli, deallocare le code di messaggi */
    for(int k=0; k<2; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);
    }



    return 0;
}