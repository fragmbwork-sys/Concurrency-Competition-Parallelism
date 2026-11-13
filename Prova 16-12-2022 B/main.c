#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

#include "procedure.h"


int main() {

	int id_queue, id_shm;
	key_t chiave_shm = IPC_PRIVATE; // TODO
	key_t chiave_mbx = IPC_PRIVATE; // TODO

	id_shm = shmget(chiave_shm, sizeof(MonitorRisultati), IPC_CREAT|0664);//TODO
	id_queue = msgget(chiave_mbx, IPC_PRIVATE|0664);// TODO


	if(id_shm<0){
		printf("[MAIN]: Errore nella creazione della memoria condivisa\n");
		exit(1);
	}


	if(id_queue<0){
		printf("[MAIN]: Errore nella creazione della mailbox\n");
		exit(1);
	}


	MonitorRisultati * ls = shmat(id_shm, NULL, 0);// TODO
	if(ls==(void*)-1){
		perror("Errore SHMAT");
		exit(1);
	}

	// TODO: inizializzare struttura
	ls->nelem= 0;
	ls->testa=0;
	ls->coda=0;
	for(int j=0; j<DIM; j++){
	ls->buffer[j]= 0;
	}
	init_monitor(&ls->m, 2);

	
	pid_t pid;

	int j;
	for(j=0; j < N_STAMPA ; j++) {
		
		// TODO
		pid=fork();
		if(pid==0){
			printf("[%d]Inizio Stampa\n",getpid());
			stampa(ls);
			exit(0);
						
		}else if(pid<0){
			printf("[MAIN]: Errore con il pid della stampa\n");
			exit(1);
		}
	}

	
	for(j=0; j < N_FILTRI ; j++) {
		
		// TODO
		pid_t pid= fork();
		if(pid==0){
			printf("[MAIN]: Creazione del processo Filtro n. %d\n", j+1);
			filtro(ls, id_queue);
			exit(0);
		}else if(pid<0){
			printf("[MAIN]: Errore con il pid del Filtro\n");
			exit(1);
		}
	}

	for(j=0; j < N_GENERATORI ; j++) {
	
		// TODO
		pid_t pid= fork();
		if(pid==0){
			printf("[MAIN]: Creazione del processo Generatore n. %d\n", j+1);
			generazione(id_queue);
			exit(0);
		}else if(pid<0){
			printf("[MAIN]: Errore con il pid del Generatore\n");
			exit(1);
		}
	
	
	}

	// TODO: attendere terminazione processi figli
	for(int k=0; k<N_STAMPA; k++){
		wait(NULL);
		printf("[MAIN]: Terminazione del processo di Stampa n. %d\n", k+1);
	}

	for(int k=0; k<N_FILTRI; k++){
		wait(NULL);
		printf("[MAIN]: Terminazione del processo Filtro n. %d\n", k+1);
	}

	for(int k=0; k<N_GENERATORI; k++){
		wait(NULL);
		printf("[MAIN]: Terminazione del processo Generatore n. %d\n", k+1);
	}
	
	// TODO: deallocazione strutture
	msgctl(id_queue, IPC_RMID, NULL);
	shmctl(id_shm, IPC_RMID, NULL);
	
}





