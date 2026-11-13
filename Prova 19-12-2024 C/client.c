#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include "header.h"

#include <wait.h>

void bambino(int queue_req, int queue_res) {
	int i;
	for (i=0; i < NUM_LETTERE; i++) {
		/*
        	TODO: invio del messaggio
    	*/

    	MessaggioRichiesta msg;
		msg.tipo_richi= RICHIESTA;
		/* TODO */msg.numero_regali = rand() % 5 + 1;
		printf("[BAMBINO]: Invio lettera a Babbo Natale chiedendo %d regali\n", msg.numero_regali);

		int result= msgsnd(queue_req, (void *)&msg, sizeof(MessaggioRichiesta)-sizeof(long), 0);

		if(result<0){
			printf("[WARNING_CLIENT]: Errore nell'invio di un messaggio di richiesta\n");
			exit(1);
		}

		MessaggioRisposta mess;
		
		int result2= msgrcv(queue_res, (void *)&mess, sizeof(MessaggioRisposta)-sizeof(long), RISPOSTA, 0);

		if(result2<0){
			printf("[WARNING_CLIENT]: Errore nella ricezione di un messaggio di risposta\n");
			exit(1);
		}		/*
        	TODO: attesa del messaggio di risposta
    	*/
		printf("[BAMBINO]: Ho ricevuto la conferma di ricezione!\n");
		sleep(1);
	}
}

int main() {
	int pid;
	/*
        TODO: recupero code di messaggi
    */

	key_t chiave_coda_richieste= ftok(".", 'f');
    key_t chiave_coda_risposte= ftok(".", 'g');

    int id_msg_richieste= msgget(chiave_coda_richieste, 0);
    int id_msg_risposte= msgget(chiave_coda_risposte, 0);

	int i;
	for (int i = 0; i < NUM_BAMBINI; i++) {
		/*
        	TODO: completare creazione figli 
    	*/pid= fork();
			if(pid==0){
				printf("[CLIENT]: Creazione dle processo bambino n. %d\n", i+1);
				bambino(id_msg_richieste, id_msg_risposte);
				exit(0);
			}else if(pid<0){
				printf("[CLIENT] Errore nella creazione di un processo bambino\n");
				exit(1);
			}		
            srand(time(NULL)*getpid());
			printf("[CLIENT]: Bambino %d sta scrivendo la letterina...\n",getpid());
			
		
	}

	/*
        TODO: attesa terminazione figli
    */
	for (int k=0; k<NUM_BAMBINI; k++){
		wait(NULL);
		printf("[CLIENT]: Terminazione del processo bambino n. %d\n", k+1);
	}

	msgctl(id_msg_richieste,IPC_RMID,0);
	msgctl(id_msg_risposte,IPC_RMID,0);

	return 0;
}
