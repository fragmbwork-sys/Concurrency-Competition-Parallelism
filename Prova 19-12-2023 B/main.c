#include "prodcons.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void * Produttore(void *);
void * Consumatore(void *);

int main() {

    /* TBD: Avviare i thread produttore e consumatore */
	//Creazione oggetto MonitorStreaming
    MonitorStreaming *monito= (MonitorStreaming*)malloc(sizeof(MonitorStreaming));
	
	//Inizializzazione m
    inizializza(monito);
	
	//Creazione thread
	pthread_t produttore;
	pthread_t consumatore;
	
	//Avvio dei thread
    printf("[MAIN]: Creazione Thread Produttore...\n");
	int err= pthread_create(&produttore, NULL, Produttore, (void*)monito);
	if(err){perror("[MAIN]: errore nella creazione del Thread Produttore\n"); exit(1);}

    printf("[MAIN]: Creazione Thread Consumatore...\n");
    int err2= pthread_create(&consumatore, NULL, Consumatore, (void*)monito);
	if(err2){perror("[MAIN]: errore nella creazione del Thread Consumatore\n"); exit(1);}

	//Terminazione dei thread
    pthread_join(produttore, NULL);
    printf("[MAIN]: Terminazione Thread Produttore\n");

    pthread_join(consumatore, NULL);
    printf("[MAIN]: Terminazione Thread Consumatore\n");
	
	//distruzione mutex e de-allocazione memoria
    distruggi(monito);
    free(monito);
	

}

void * Produttore(void * p) {
	
	MonitorStreaming * m = (MonitorStreaming *) p;

    for(int i=0; i<10; i++) {
		
		//printf("Inizio produttore #%d \n",i);

        Frame f;
        f[0][0] = rand() % 11;
        f[0][1] = rand() % 11;
        f[1][0] = rand() % 11;
        f[1][1] = rand() % 11;

        /* TBD: Chiamare il metodo "produci()" del monitor */
		produci(m, f);

        sleep(1);
    }

    pthread_exit(NULL);
}

void * Consumatore(void * p) {
	
	MonitorStreaming * m = (MonitorStreaming *) p;

    /* TBD: Chiamare il metodo "bufferizza()" del monitor */
	int n = 4;
    bufferizza(m, n);
	

    for(int i=0; i<10; i++) {
		
		//printf("Inizio consumatore #%d \n",i);

        Frame f;

        /* TBD: Chiamare il metodo "consuma()" del monitor */
        consuma(m, f);
		

        sleep(1);
    }

    pthread_exit(NULL);
}