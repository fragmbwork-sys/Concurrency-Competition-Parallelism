#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


int main(){
	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = /*TODO*/ftok(".", 'f');

	int ds_shm = /*TODO*/shmget(key_shm, sizeof(struct Coda), 0);

	if(ds_shm<0){
		printf("[CLIENT]: Errore nelle creazione della coda condivisa\n");
		exit(1);
	}

	struct Coda * c;
	c = /*TODO*/shmat(ds_shm, NULL, 0);

	if(c==(void*)-1){
		printf("[MAIN]: Errore durante l'attach della memoria condivisa\n");
		exit(1);
	}
	
	//ottenimento semafori
	if(c==(void*)-1){
		printf("[CLIENT]: Errore durante l'attach della memoria condivisa\n");
		exit(1);
	}

	key_t key_sem;
	key_sem = /*TODO*/ftok(".", 'g');

	int ds_sem = /*TODO*/semget(key_sem, 4, 0);


	if(ds_sem<0){
		printf("[MAIN]: Errore nelle creazione della coda condivisa\n");
		exit(1);
	}

    
	printf("[Client %d] - invio richieste...\n",getpid());

	int i = 0;
	int num_prenotazioni;	


	while(i < NUM_REQ){
		num_prenotazioni = (rand()%5)+1;//TODO numero casuale
		//TODO invio richiesta tramite funzione "produci"
		produci(c, ds_sem, num_prenotazioni);
	
		sleep(1);
		i++;
	}
	//TODO invio richiesta di terminazione (-1)
	printf("[CLIENT]: Invio richiesta di prenotazione con valore -1\n");
	produci(c, ds_sem, -1);
	

	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}
