#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include "header.h"


int main(int argc, char *argv[]){
	int coda;
	pid_t pid;
	long id_sensore;
        
	if (argc < 2 ) {
		printf("ERRORE: il client deve ricevere il proprio ID\n");
		return -1;
	}
        
	id_sensore = atol(argv[1]);
       
	key_t key;
	key = ftok("./", 'f');//TODO:inserire la chiave
    
	coda = msgget(key, 0);//TODO:inizializzare la coda

	printf("[DEBUG] - sensore id coda %d\n",coda);	
	printf("[Client %d] - invio richieste...\n",getpid());


	int i = 0;
	Misura misura;	

	srand(time(NULL));
	
	// TODO: inviare in cilco NUM_REQ misure da inviare al server tramite coda
	while(i<NUM_REQ){
		misura.Client= id_sensore;
		misura.misura= rand()%101;

		int result= msgsnd(coda, (void *)&misura, sizeof(Misura)-sizeof(long), 0);

		if(result<0){perror("[CLIENT]: Errore nell'invio di un messaggio\n"); exit(1);}

		printf("[CLIENT]: Messaggio inviato con id_sensore: %ld e misura: %d\n", id_sensore, misura.misura);

		i++;

	}


	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}
