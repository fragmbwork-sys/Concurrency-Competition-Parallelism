#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){
	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = /*TODO*/ftok(".", 'c');

	int ds_shm = /*TODO*/shmget(key_shm, sizeof(struct Coda), IPC_CREAT|0664);

	if(ds_shm<0){
		printf("[WARNING_MAIN]: Errore nella creazione della memoria condivisa\n");
		exit(1);
	}

	struct Coda * c;
	c = /*TODO*/shmat(ds_shm, NULL, 0);

	if(c==(void*)-1){

		printf("[WARNING_MAIN]: Errore durante l'attach della risorsa conivisa\n");
		exit(1);
	}

	c->testa= 0;
	c->coda= 0;
	
	//ottenimento array semafori

	key_t key_sem;
	key_sem = /*TODO*/ftok(".", 's');

	int ds_sem = /*TODO*/semget(key_sem, 4, IPC_CREAT|0664);

	if(ds_sem<0){
		printf("[WARNING_MAIN]: Errore nella creazione dei semafori\n");
		exit(1);
	}
	
	//TODO inizializzazione coda e semafori
	semctl(ds_sem, MUTEX_P, SETVAL, 1);
	semctl(ds_sem, MUTEX_C, SETVAL, 1);
	semctl(ds_sem, SPAZIO_DISP, SETVAL, DIM_BUFFER);
	semctl(ds_sem, MESS_DISP, SETVAL, 0);
  
	int i = 0;
	int pid;
	int status;
	
	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){
		
		sleep(1);

		pid = fork();

		if(pid == 0){

			if(i == NUM_CLIENT+NUM_SERVER -1){
				//TODO aggiungere exec per avviare il server
				if(execl("./server", "server", NULL)<0){
					printf("[WARNING_MAIN]: Errore nella creazione del processo Server\n");
					exit(1);
				}
			}else{
				//TODO aggiungere exec per avviare il client
				if(execl("./client", "client", NULL)<0){
					printf("[WARNING_MAIN]: Errore nella creazione del processo Client\n");
					exit(1);
				}
				//exit(-1);
			}
		}
	}

	for(i = 0; i < NUM_CLIENT+NUM_SERVER;i++){

		wait(&status);

		if (WIFEXITED(status)) {
    			printf("[PADRE] - Figlio terminato con stato %d\n",status);
  		}
	}

	//TODO rimozione array semafori e memoria condivisa
	semctl(ds_sem, 0, IPC_RMID);
	shmctl(ds_shm, IPC_RMID, 0);

	printf("[PADRE] - Fine elaborazione...\n");

	return 0;	
}
