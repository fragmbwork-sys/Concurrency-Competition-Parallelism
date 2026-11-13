#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdlib.h>

int main(){
    int pid;
    key_t msg_req_key = ftok("./", 'f');//TODO: definire chiave
	key_t msg_res_key = ftok("./", 'g');//TODO: definire chiave

    //TODO: INIZIALIZZAZIONE CODE
    int id_msg_req= msgget(msg_req_key, IPC_CREAT|0664);
    int id_msg_res= msgget(msg_res_key, IPC_CREAT|0664);

    if(id_msg_req<0){
        printf("[MAIN]: Errore nella creazione della coda delle richieste\n");
        exit(1);
    }

    if(id_msg_req<0){
        printf("[MAIN]: Errore nella creazione della coda delle risposte\n");
        exit(1);
    }
	



    pid = fork();
    if (pid==0) {
        if(execl("./server","./server",NULL)<0){perror("[MAIN]: Errore nella creazione del processo server\n"); exit(1);}
        _exit(0);
    }

    pid = fork();
    if (pid==0) {
        if(execl("./reader","./reader",NULL)<0){perror("[MAIN]: Errore nella creazione del processo reader\n"); exit(1);}
        _exit(0);
    }

    wait(NULL);
    wait(NULL);

    // TODO: eliminazione code
    shmctl(id_msg_req, IPC_RMID, NULL);
    shmctl(id_msg_res, IPC_RMID, NULL);

    return 0;
}
