#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){
	int i,status;
	int coda;
	pid_t pid;

	key_t key;
	key = ftok("./", 'f');//TODO:inserire la chiave
    
	coda = msgget(key, IPC_CREAT|0664);//TODO:inizializzare la coda

	
	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){
		
		char id_client = '1'+i;
		sleep(1);
		
		pid = fork();

		if(pid == 0){

			if(i == NUM_CLIENT+NUM_SERVER -1){
				printf("[SERVER %d] - Sono stato creato...\n",getpid());
				//TODO creare server con exec
				if(execl("./server", "server", NULL)<0){perror("[MAIN]: Errore nella creazione del Server\n");}
				exit(-1);
			}else{
				printf("[CLIENT %d] - Sono stato creato con ID %c\n",getpid(),id_client);
				//TODO creare client con exec, passando anche il parametro &id_client
				if(execl("./client", "client", &id_client, NULL)<0){perror("[MAIN]: Errore nella creazione del Client\n");}
				exit(-1);
			}
		}
	}

	for(i = 0; i < NUM_CLIENT+NUM_SERVER; i++){

		wait(&status);

		if (WIFEXITED(status)) {
    			printf("[PADRE] - Figlio %d terminato con stato %d\n", i+1, status);
  		}
	}

	//TODO eliminare la coda
	msgctl(coda, IPC_RMID, NULL);

	printf("[PADRE] - Fine elaborazione...\n");

	return 0;	
}
