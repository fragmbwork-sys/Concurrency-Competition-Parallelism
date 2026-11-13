#include "header.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>

void * create_coordinates (void * p){
	struct coordinates c;
	// TODO: ottenimento parametri passati: monitor e coordinata x
	struct parameters * par= (struct parameters*) p;
	struct monitor *m= par->m;

	printf("[Create: Started create_coordinates, randint: %d]\n",par->x);

	c.x= par->x;
	
	sleep(1);
	for(int i=0; i<N_COMPUTE_PER_THREAD; i++){
		c.y = rand()%100;
		printf("[Create: Created coordinates: %d, %d]\n",c.x, c.y);
		// TODO: inserimento delle coordinate nel monitor

		send_coordinates(par->m, &(c)); //oppure m, otteniamo lo stesso risultato

		sleep(1);
	}
	// TODO: terminazione thread
	pthread_exit(NULL);
}

void * send_values (void * p){
	// TODO: creazione coda di messaggi

	key_t chiave_coda= ftok(".", 'b');
	int id_msg= msgget(chiave_coda, IPC_CREAT|0664);

	if(id_msg<0){
		printf("[WARNING_SENDVALUE]: Errore nella creazione della coda di messaggi\n");
		exit(1);
	}


	// TODO: ottenimento parametri passati: monitor
	struct monitor *m= (struct monitor*) p;

	printf("[Send: Started send_values]\n");
	struct coordinates c;

	sleep(1);
	for(int i=0; i<N_COMPUTE_PER_THREAD*NTHREADS_PROD; i++){
		
		// TODO: Consuma coordinate prese dal monitor
		receive_coordinates(m, &(c));
		printf("[Consumed coordinates: %d, %d]\n",c.x, c.y);
		// TODO: creazione e invio messaggio sulla coda
		struct message m;

		m.tipo= 2;
		m.valore.x= c.x;
		m.valore.y= c.y;

		int result= msgsnd(id_msg, (void*)&m, sizeof(struct message)-sizeof(long), 0);

		if(result<0){perror("[WARNING_SENDVALUES]: Errore nell'invio di un messaggio\n");}


	}

	// TODO: rimozione coda
	msgctl(id_msg, IPC_RMID, 0);
	// TODO: terminazione thread
	pthread_exit(NULL);
}

void * rate_limiter_loop (void * p){
	// TODO: ottenimento parametri passati: monitor
	struct monitor* mon= (struct monitor*) p;

	printf("[Create: Started rate_limiter]\n");

	for(int i=0; i<15; i++){
		// TODO: reset rate limiting del monitor 
		reset_rate(mon);
		printf("\t\t[Resetting limits]\n");
		sleep(1);
	}
	// TODO: terminazione thread
	pthread_exit(NULL);
}

int main() {

	srand(time(NULL));

	// TODO: Creazione attributi task
	pthread_attr_t attr;
	pthread_attr_init(&(attr));

	pthread_attr_setdetachstate(&(attr), PTHREAD_CREATE_JOINABLE);

	// TODO: Creazione variabili necessarie per pthreads
	pthread_t vettore_id_prod[NTHREADS_PROD];
	pthread_t id_cons;
	pthread_t id_rate;

	// TODO: Creazione struttura per passaggio parametri a thread
	struct parameters parametri[NTHREADS_PROD];


	// TODO: Allocazione monitor
	struct monitor *variabile= (struct monitor*) malloc(sizeof(struct monitor));
	inizializza(variabile);

	int i;

	// TODO: creazione thread rate_limiter
	pthread_create(&id_rate, &(attr), (void*) rate_limiter_loop, (void*) variabile);

	sleep(1);

	
	for(i=0;i<NTHREADS_PROD;i++){
		// TODO: riempimento parametri threads (inclusa la coordinata x)
		parametri[i].x= i;
		parametri[i].m= variabile;
		
		// TODO: creazione thread produttore
		pthread_create(&(vettore_id_prod[i]), &(attr), (void*)create_coordinates, (void*)&parametri[i]);
	}

	// TODO: creazione thread consumatore
	pthread_create(&id_cons, &(attr), (void*)send_values, (void*)variabile);

	for(i=0;i<NTHREADS_PROD;i++){
		// TODO: attesa thread produttore
		pthread_join(vettore_id_prod[i], NULL);
		printf("Producer %d terminato\n",i+1);
	}

	// TODO: attesa altri thread
	pthread_join(id_cons, NULL);
	printf("Consumer terminato\n");
	pthread_join(id_rate, NULL);
	printf("Rate limiter terminato\n");

	// TODO: rimozione monitor
	rimuovi(variabile);
	free(variabile);

	return 0;
}	
