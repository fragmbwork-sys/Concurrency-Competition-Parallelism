#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header_msg.h"

int main() {

	
    /* TBD: Creare una coppia di code di messaggi UNIX */
    key_t key_send= ftok("./", 'f');
    key_t key_rcv= ftok("./", 'g');
    
    
    int send_queue=msgget(key_send, IPC_CREAT| 0664);
    int rcv_queue=msgget(key_rcv, IPC_CREAT | 0664);
    
    
    if(send_queue<0){
    	perror("Errore nella creazione della coda sender\n");
    	exit(1);
    }
	
	if(rcv_queue<0){
    	perror("Errore nella creazione della coda receiver\n");
    	exit(1);
    }
	

    /* TBD: Creare un processo figlio, che esegua l'eseguibile "server" */
    pid_t pid=fork();
    if(pid==0){

        if(execl("./server", "server", NULL)<0){perror("[MAIN]: Errore nella creazione del processo Server\n"); exit(1);}
    }else if(pid<0){
        printf("[MAIN]: Errore con il pid del processo Server\n");
        exit(1);
    }
    


    /* TBD: Creare 2 processi figli, che eseguano l'eseguibile "client" */

    for(int i=0; i<2; i++){

        pid_t pid=fork();

        if(pid==0){

        if(execl("./client", "client", NULL)<0){perror("[MAIN]: Errore nella creazione del processo Client\n"); exit(1);}
    }else if(pid<0){
        printf("[MAIN]: Errore con il pid del processo Client\n");
        exit(1);
    }

    }
	
	
	/* TBD: Attendere la terminazione dei figli, deallocare le code di messaggi */
    for(int k=0; k<3; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);

    }

    msgctl(send_queue, IPC_RMID, NULL);
    msgctl(rcv_queue, IPC_RMID, NULL);

    
    return 0;
}
