#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#include "header.h"


int main(){
	pid_t pid_ex,pid;
	int st,i;
	int shmid,semid;
	BufferMutuaEx* buf;

	key_t key_shm,key_sem;
	key_shm = IPC_PRIVATE;// TODO 
	key_sem = IPC_PRIVATE;// TODO 

	shmid = shmget(key_shm, sizeof(BufferMutuaEx), IPC_CREAT|0664);// TODO 
	semid = semget(key_sem, 5, IPC_CREAT|0664);// TODO 

	if(shmid<0){
		printf("[MAIN]: Errore nella creazione della memoria condivisa\n");
		exit(1);
	}
	
	if(semid<0){
		printf("[MAIN]: Errore nella creazione dei semafori\n");
		exit(1);
	}

	printf("[MASTER] - Shmid : %d, Semid: %d\n",shmid,semid);

	// TODO: inizializzare semafori
	semctl(semid, MESS1, SETVAL, 0);
	semctl(semid, MESS2, SETVAL, 0);
	semctl(semid, SPAZIO_DISP, SETVAL, DIM);
	semctl(semid, MUTEX_P, SETVAL,  1);
	semctl(semid, MUTEX_C, SETVAL, 1);

	buf = shmat(shmid, NULL, 0);// TODO 

	if(buf==(void*)-1){perror("[MAIN]: Errore nell'attach della risorsa condivisa\n"); exit(1);}
	
	// TODO: inizializzazione della struttura
	for(int j=0; j<DIM; j++){
		buf->stato[j]= LIBERO;
	}

	for(int j=0; j<DIM; j++){
		buf->vettore[j]= 0;
	}
	


	srand(time(NULL));

	for(i = 0; i < NPROD; i++){
		pid = fork();
		
		// TODO: ogni figlio produttore (1 e 2) fa PRODS produzioni
		if(pid==0){
			
			if(i%2==0){
				printf("[MAIN]: Creazione del Produttore di tipo 1...\n");
				for(int l=0; l<PRODS; l++){
				produci_tipo_1(buf, semid, OCCUPATO_1);
				}
				exit(0);
			}else{
				printf("[MAIN]: Creazione del Produttore di tipo 2...\n");
				for(int l=0; l<PRODS; l++){
				produci_tipo_2(buf, semid, OCCUPATO_2);
				}
				exit(0);
			}
	
		}else if(pid<0){
			printf("[MAIN]: Errore con il pid dei produttori\n");
			exit(1);
		}

	}

	for(i = 0; i < NCONS; i++){
		pid = fork();

		// TODO: ogni figlio consumatore (1 e 2) fa CONS consumi
		if(pid==0){
			
			if(i%2==0){
				printf("[MAIN]: Creazione del Consumatore di tipo 1...\n");
				for(int l=0; l<CONS; l++){
				consuma_tipo_1(buf, semid);
				}
				exit(0);
			}else{
				printf("[MAIN]: Creazione del Consumatore di tipo 2...\n");
				for(int l=0; l<CONS; l++){
				consuma_tipo_2(buf, semid);
				}
				exit(0);
			}
	
		}else if(pid<0){
			printf("[MAIN]: Errore con il pid dei consumatori\n");
			exit(1);
		}

	}
	
	// TODO: attesa completamento figli
	for(int k=0; k<NPROD; k++){
		wait(NULL);
		printf("[MAIN]: Terminazione del processo produttore n. %d\n", k+1);
	}


	for(int k=0; k<NCONS; k++){
		wait(NULL);
		printf("[MAIN]: Terminazione del processo consumatore n. %d\n", k+1);
	}

	// TODO: rimozione strutture
	semctl(semid, 0, IPC_RMID);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}

