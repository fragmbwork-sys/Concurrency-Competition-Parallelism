#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include "header.h"

#define NUM_WORKER 3

static struct Coda * coda;
static int ds_sem;
static pthread_t threads[NUM_WORKER];
static int end = 0;

// struttura per passaggio parametri ai thread
typedef struct {
	int id;
	Count* count;
} parametri;

void init_buffer(Count* c){
	//TODO aggiungere codice per inizializzare Count
	c->num_prenotazioni= 0;
	c->count= 0;
	pthread_mutex_init(&c->mutex, NULL);
}

void remove_buffer(Count* c){
	//TODO aggiungere codice per rimuovere Count
	pthread_mutex_destroy(&c->mutex);
	//free(c);
}


void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	parametri* par = /*TODO*/(parametri*) arg;
	//TODO: completare il passaggio dei parametri
	Count *c= (Count*) par->count;
	
	while (!end) {
		//TODO completare il codice del worker
		// worker preleva dalla coda con funzione "consuma"
		int end= consuma(coda, ds_sem);
		// aggiornamento (eventuale) variabile end se valore ricevuto = -1
		if(end== -1){
			printf("[SERVER]: Ricevuto Richiesta con valore -1\n");
			break;
		}
		// aggiornamento Count in mutua esclusione	
		pthread_mutex_lock(&c->mutex);

		c->count= c->count+end;

		printf("[SERVER]: Variabile Count aggiornata, valore attuale: %d\n", c->count);

		pthread_mutex_unlock(&c->mutex);
	}
    //TODO uscita dal thread
	pthread_exit(NULL);
}



int main(){

	int i,k;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = /*TODO*/ftok(".", 'f');

	int ds_shm = /*TODO*/shmget(key_shm, sizeof(struct Coda), 0);

	coda = /*TODO*/shmat(ds_shm, NULL, 0);

	if(coda==(void*)-1){
		printf("[MAIN]: Errore durante l'attach della memoria condivisa\n");
		exit(1);
	}
	
	//ottenimento semafori

	key_t key_sem;
	key_sem = /*TODO*/ftok(".", 'g');

	ds_sem = /*TODO*/semget(key_sem, 4, 0);

	//TODO Creazione e Inizializzazione struttura locale
	Count* count = /*TODO*/(Count*)malloc(sizeof(Count));
	
	parametri param[NUM_WORKER];

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER;i++){
		printf("[SERVER] - creo thread %d...\n",i);
		//TODO creazione threads
		param[i].id= i;	
		param[i].count= count;
		pthread_create(&threads[i], &attr, worker, (void *) &param[i]);
	}
	
	for(k=0; k < NUM_WORKER; k++) {
	  //TODO join 
	  pthread_join(threads[k], NULL);
	  printf("[SERVER]: Terminazione del Thread n. %d\n", k);

	}
	
	//TODO rimozione struttura locale
	remove_buffer(count);
	free(count);

	printf("[SERVER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

