#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header_msg.h"
#include "header_sem.h"


int main() {

    srand(getpid());


    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t key_send= ftok("./", 'f');
    key_t key_rcv= ftok("./", 'g');
    
    
    int send_queue=msgget(key_send, 0);
    int rcv_queue=msgget(key_rcv, 0);

	
     if(send_queue<0){
    	perror("Errore nella creazione della coda sender lato client\n");
    	exit(1);
    }
	
	if(rcv_queue<0){
    	perror("Errore nella creazione della coda receiver lato client\n");
    	exit(1);
    }

    int num_valori = 10;

    msg_init_request req;
    msg_init_response res;
	

    printf("[CLIENT %d] Invio richiesta (num. valori: %d)\n", getpid(), num_valori);

    /* TBD: Inviare il messaggio di richiesta */
    msg_init_request mess;
    mess.tipo= MSG_TYPE;
    mess.valori= num_valori;
    mess.pid= getpid();

    int result= msgsnd(send_queue, (void *)&mess, sizeof(msg_init_request)-sizeof(long), 0);

    if(result<0){perror("[CLIENT]: Errore nel'invio della richiesta al Server\n"); exit(1);}
    
	

    /* TBD: Ricevere il messaggio di risposta */
    msg_init_response risposta;
    result= msgrcv(rcv_queue, (void*)&risposta, sizeof(msg_init_response)-sizeof(long), getpid(), 0);
    if(result<0){perror("[CLIENT]: Errore nella ricezione di un messaggio dal Server\n"); exit(1);}


    printf("[CLIENT %d] Ricevuto risposta\n", getpid());


    int id_shm_invio =/* TBD */risposta.shm_invio;
    int id_sem_invio =/* TBD */risposta.sem_invio;

    int id_shm_ricezione =/* TBD */risposta.shm_ricezione;
    int id_sem_ricezione =/* TBD */risposta.sem_ricezione;
    
	prodcons* p1;
	prodcons* p2;
	
	p1=(prodcons*) shmat(id_shm_invio, NULL, 0);
	p2=(prodcons*) shmat(id_shm_ricezione, NULL, 0);
    for(int i=0; i<num_valori; i++) {

        int valore = rand() % 10;


        produci(id_sem_invio, 	p1, valore);

        printf("[CLIENT %d] Valore inviato: %d\n", getpid(), valore);


        int ricevuto = consuma(id_sem_ricezione,p2 );

        printf("[CLIENT %d] Valore ricevuto: %d\n", getpid(), ricevuto);

    }

    //semctl(id_sem_invio, 0, IPC_RMID);
    //semctl(id_sem_ricezione, 0, IPC_RMID);

    //shmctl(id_shm_ricezione, IPC_RMID, NULL);
    ///shmctl(id_shm_invio, IPC_RMID, NULL);

}
