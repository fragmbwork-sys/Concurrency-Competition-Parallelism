#include "header.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	int pid;

	//TODO: creazione processo startPL
	pid = fork();
	if (pid == 0){
		if(execl("./startPL", "startPL", NULL)<0) {
			printf("[WARNING_START]: Errore nella creazione di START_PL");
			exit(1);
		}

	} else if (pid < 0){
		printf("[WARNING_START]: Errore crezione eseguibile startPL. \n");
	}

	//TODO: creazione processo startRL
	pid = fork();
	if (pid == 0){
		if(execl("./startRL", "startRL", NULL)<0) {
			printf("[WARNING_START]: Errore nella creazione di START_RL");
			exit(1);
		}

	} else if (pid < 0){
		printf("[WARNING_START] Errore creazione eseguibile startRL. \n");
	}
	
	//TODO: attesa processi figlio
	for(int k=0; k<2; k++){
		wait(NULL);
		printf("[START]: Terminazione dell'eseguibile n. %d\n", k+1);
	}
	
	return 0;
}	
