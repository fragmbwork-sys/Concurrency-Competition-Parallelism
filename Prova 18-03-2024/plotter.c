#include "header.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>


int main() {
	// TODO: creazione coda di messaggi
	key_t chiave_coda= ftok(".", 'b');
	int id_msg= msgget(chiave_coda, IPC_CREAT|0664);

	if(id_msg<0){

		printf("[WARNING_PLOTTER]: Errore nella creazione della coda\n");
		exit(1);
	}

	struct coordinates c;
	struct message mess;
	int ret;

	sleep(1);

	for(int i=0; i<N_COMPUTE_PER_THREAD*NTHREADS_PROD; i++){
		// TODO: ricezione messaggio dalla coda

		int result= msgrcv(id_msg, (void*)&mess, sizeof(struct message)-sizeof(long), 2, 0);

		if(result<0){perror("[WARNING_PLOTTER]: Errore nella ricezione di un messaggio\n");}

		c.x= mess.valore.x;
		c.y= mess.valore.y;

		printf("[Plotter: %d, %d]\n",c.x, c.y);
	}

	// TODO: rimozione coda
	msgctl(id_msg, IPC_RMID, 0);

	return 0;
}	
