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
	c->num_prenotazioni=0;
	pthread_mutex_init(&(c->mutex), 0);
}

void remove_buffer(Count* c){
	//TODO aggiungere codice per rimuovere Count
	pthread_mutex_destroy(&(c->mutex));
}


void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	parametri* par = /*TODO*/(parametri*) arg;
	//TODO: completare il passaggio dei parametri
	Count * c= par->count;
	int id= par->id;
	
	while (!end) {
		//TODO completare il codice del worker
		// worker preleva dalla coda con funzione "consuma"
		int valore= consuma(coda, ds_sem);
		// aggiornamento (eventuale) variabile end se valore ricevuto = -1
		if(valore== -1){
			end= 1;
			printf("[WORKER]: Ricevuta richiesta di fine, termino tutti\n");
			break;
		}
		// aggiornamento Count in mutua esclusione	
		pthread_mutex_lock(&(c->mutex));
		c->num_prenotazioni += valore;
		printf("[WORKER %d] Variabile aggiornata %d\n", id, c->num_prenotazioni);
		pthread_mutex_unlock(&(c->mutex));

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
	key_shm = /*TODO*/ftok(".", 'c');

	int ds_shm = /*TODO*/shmget(key_shm, sizeof(struct Coda), 0);

	if(ds_shm<0){
		printf("[WARNING_SERVER]: Errore nella creazione della memoria condivisa\n");
		exit(1);
	}

	coda = /*TODO*/shmat(ds_shm, NULL, 0);

	if(coda==(void*)-1){
		printf("[WARNING_SERVER: Errore nell'attach della memoria\n");
		exit(1);
	}
	
	//ottenimento semafori

	key_t key_sem;
	key_sem = /*TODO*/ftok(".", 's');

	ds_sem = /*TODO*/semget(key_sem, 4, 0);

	if(ds_sem<0){
		printf("[WARNING_SERVER: Errore nella creazione dei semafori\n");
		exit(1);
	}

	//TODO Creazione e Inizializzazione struttura locale
	Count* count = /*TODO*/malloc(sizeof(Count));
	init_buffer(count);
	
	parametri param[NUM_WORKER];

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER;i++){
		printf("[SERVER] - creo thread %d...\n",i);
		//TODO creazione threads
		param[i].id= i;
		param[i].count= count;	
		int error= pthread_create(&threads[i], &attr, worker, (void*)&param[i]);

		if(error){
			printf("[WARNING_SERVER: Errore nella creazione del Thread\n");
			exit(1);
		}
	}
	
	for(k=0; k < NUM_WORKER; k++) {
	  //TODO join 
	  pthread_join(threads[k], NULL);
	}
	
	//TODO rimozione struttura locale
	remove_buffer(count);
	free(count);

	printf("[SERVER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

