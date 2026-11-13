#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header_msg.h"
#include "header_sem.h"


void server(/* TBD: Completare il passaggio di parametri */prodcons* p1,prodcons* p2, int val, int id_sem1, int id_sem2);


int main() {


    /* TBD: Ottenere gli identificativi delle code di messaggi */
	key_t key_send= ftok("./", 'f');
    key_t key_rcv= ftok("./", 'g');
    
    
    int send_queue=msgget(key_send, 0);
    int rcv_queue=msgget(key_rcv, 0);
	


    for(int richieste=0; richieste<2; richieste++) {

        msg_init_request req;
        msg_init_response res;


        /* TBD: ricevere un messaggio dal client */
		int result= msgrcv(send_queue, (void*)&req, sizeof(msg_init_request)-sizeof(long), MSG_TYPE, 0);
    	if(result<0){perror("[SERVER]: Errore nella ricezione di un messaggio dal Client\n"); exit(1);}
       

        int num_valori =/* TBD */req.valori;
        int pid_client =/* TBD */req.pid;

        printf("[SERVER %d] Ricezione richiesta (num. valori: %d)\n", getpid(), num_valori);


        /* TBD: Allocare e inizializzare le risorse che saranno condivise
                tra il client e il processo server figlio
         */
		
        int id_sem_invio =/* TBD */semget(IPC_PRIVATE, 4, IPC_CREAT|0664);
        int id_shm_invio =/* TBD */shmget(IPC_PRIVATE, sizeof(prodcons), IPC_CREAT|0664);

        int id_sem_ricezione =/* TBD */semget(IPC_PRIVATE, 4, IPC_CREAT|0664);
        int id_shm_ricezione =/* TBD */shmget(IPC_PRIVATE, sizeof(prodcons), IPC_CREAT|0664);

		if(id_shm_invio<0){
			perror("Errore nel descrittore shared memory lato server\n");
			exit(1);
		
		}
		if(id_shm_ricezione<0){
			perror("Errore nel descrittore shared memory lato server\n");
			exit(1);
		
		}
		if(id_sem_invio<0){
			perror("Errore nel descrittore semaforo 1 lato server\n");
			exit(1);
		
		}
		
		if(id_sem_ricezione<0){
			perror("Errore nel descrittore semaforo 2 lato server\n");
			exit(1);
		
		}
		
		semctl(id_sem_invio, SPAZIO_DISPONIBILE, SETVAL, DIM);
		semctl(id_sem_invio, MESSAGGIO_DISPONIBILE, SETVAL, 0);
		semctl(id_sem_invio, MUTEXP, SETVAL, 1);
		semctl(id_sem_invio, MUTEXC, SETVAL, 1);
		
		semctl(id_sem_ricezione, SPAZIO_DISPONIBILE, SETVAL, DIM);
		semctl(id_sem_ricezione, MESSAGGIO_DISPONIBILE, SETVAL, 0);
		semctl(id_sem_ricezione, MUTEXP, SETVAL, 1);
		semctl(id_sem_ricezione, MUTEXC, SETVAL, 1);

        /* TBD: Inviare il messaggio di risposta al client, 
                con gli identificativi delle risorse condivise
         */

		msg_init_response risposta;
		risposta.tipo_risp= pid_client;
		risposta.shm_ricezione= id_shm_ricezione;
		risposta.shm_invio= id_shm_invio;
		risposta.sem_invio= id_sem_invio;
		risposta.sem_ricezione= id_sem_ricezione;

		result= msgsnd(rcv_queue, (void *)&risposta, sizeof(msg_init_response)-sizeof(long), 0);
		if(result<0){perror("[SERVER]: Errore nell'invio di un messaggio di risposta al Client\n"); exit(1);}

        printf("[SERVER %d] Inviato risposta\n", getpid());

    	/*ret=msgsnd(rcv_queue, &res, sizeof(msg_init_response)-sizeof(long), 0);
    	if(ret<0){
    		perror("Errore nella send lato server\n");
    		exit(1);
    	}*/
    	
    	prodcons* p1;
    	prodcons* p2;
    	
    	p1=(prodcons*)shmat(id_shm_invio, NULL, 0);
    	p2=(prodcons*)shmat(id_shm_ricezione, NULL, 0);
    	p1->testa=0;
    	p1->coda=0;
    	p2->testa=0;
    	p2->coda=0;

        /* TBD: Creare un processo server figlio, e
                passargli i riferimenti alle risorse condivise
         */
		
		pid_t pid= fork();

		if(pid==0){
			printf("[SERVER]: Crezione del processo server figlio...\n");
			server(p1, p2, num_valori, id_sem_invio, id_sem_ricezione);
			exit(0);
		}

		wait(NULL);

		semctl(id_sem_invio, 0, IPC_RMID);
    	semctl(id_sem_ricezione, 0, IPC_RMID);

		shmctl(id_shm_ricezione, IPC_RMID, NULL);
		shmctl(id_shm_invio, IPC_RMID, NULL);

    }
	

	
	//while(wait(NULL)>0);

	//return 0;
	

}



void server(prodcons* p1,prodcons* p2, int val, int id_sem1, int id_sem2) {

    int num_valori = /* TBD */val;

    for(int i=0; i<num_valori; i++) {

        int ricevuto = consuma(id_sem1,p1);

        printf("[SERVER %d] Valore ricevuto: %d\n", getpid(), ricevuto);

        int valore = ricevuto * 2;

        produci(id_sem2, p2, valore);
        printf("[SERVER %d] Valore inviato: %d\n", getpid(), valore);

    }
    
    //semctl(id_sem1, 0, IPC_RMID);
    //semctl(id_sem2, 0, IPC_RMID);

}
