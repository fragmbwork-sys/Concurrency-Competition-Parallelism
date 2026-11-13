#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "prodcons.h"


void produttore_1(MonitorPC * m)
{
	srand(getpid());

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 1 */
		produci_tipo_1(m, valore);

		sleep(1);
	}

}


void produttore_2(MonitorPC * m)
{
    srand(getpid());

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 2 */
		produci_tipo_2(m, valore);

		sleep(1);
	}

}


void consumatore_1(MonitorPC * m)
{

	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
		consuma_tipo_1(m, &valore);

		sleep(1);
	}

}


void consumatore_2(MonitorPC * m)
{
	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
		consuma_tipo_2(m, &valore);

		sleep(1);
	}
}

int main(int argc, char *argv[])
{

	/* TBD: creare e inizializzare l'oggetto monitor (metodo "inizializza") */
	int id_shm= shmget(IPC_PRIVATE, sizeof(MonitorPC), IPC_CREAT|0664);
    MonitorPC* m= (MonitorPC*) shmat(id_shm, NULL, 0);

	if(m==(void*)-1){perror("[MAIN]: Errore nell'attach della risorsa condivisa\n"); exit(1);}

	inizializza(m);

	int i;

	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 1 */
		pid_t pid=fork();
		if(pid==0){
			printf("[MAIN]: Creo il processo Produttore 1 n. %d\n", i+1);
			produttore_1(m);
			exit(0);
		}else if(pid<0){
			perror("[MAIN]: Errore con il pid del Produttore 1\n");
			exit(1);
		}
	}

	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 2 */
		pid_t pid=fork();
		if(pid==0){
			printf("[MAIN]: Creo il processo Produttore 2 n. %d\n", i+1);
			produttore_2(m);
			exit(0);
		}else if(pid<0){
			perror("[MAIN]: Errore con il pid del Produttore 2\n");
			exit(1);
		}
	}

	/* TBD: avviare consumatore di tipo 1 */
	//int valore= 0;
	pid_t pid=fork();
		if(pid==0){
			printf("[MAIN]: Creo il processo Consumatore 1 \n");
			consumatore_1(m);
			exit(0);
		}else if(pid<0){
			perror("[MAIN]: Errore con il pid del Consumatore 1\n");
			exit(1);
		}


	/* TBD: avviare consumatore di tipo 2 */
	pid_t pid2=fork();
		if(pid2==0){
			printf("[MAIN]: Creo il processo Consumatore 2 \n");
			consumatore_2(m);
			exit(0);
		}else if(pid2<0){
			perror("[MAIN]: Errore con il pid del Consumatore 2\n");
			exit(1);
		}



	for(i=0; i<2; i++) {

		/* TBD: attendere terminazione produttori di tipo 1 */
		wait(NULL);
		printf("[MAIN]: Produttore di tipo 1 n. %d terminato\n", i+1);

	}


	for(i=0; i<2; i++) {

		/* TBD: attendere terminazione produttori di tipo 2 */
		wait(NULL);
		printf("[MAIN]: Produttore di tipo 2 n. %d terminato\n", i+1);
	}

	/* TBD: attendere terminazione consumatori di tipo 1 */
	wait(NULL);
	printf("[MAIN]: Consumatore 1 terminato\n");

	/* TBD: attendere terminazione consumatori di tipo 2 */
	wait(NULL);
	printf("[MAIN]: Consumatore 2 terminato\n");

	

	/* TBD: rimuovere l'oggetto monitor */
	shmctl(id_shm, IPC_RMID, NULL);

	return 0;
}

