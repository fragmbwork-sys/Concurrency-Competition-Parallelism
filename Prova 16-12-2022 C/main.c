#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

	int key_coda = ftok(FTOK_PATH, FTOK_CHAR);// TODO
    int id_coda_sensore = msgget(key_coda, IPC_CREAT|0664);//TODO

    if(id_coda_sensore < 0) {
        perror("Errore msgget");
        exit(1);
    }

    pid_t pid;

    pid = fork();

    if(pid == 0) {
        // TODO: processo figlio sensore (eseguibile distinto)
        printf("[MAIN]: Creazione del Processo Sensore...\n");   
        if(execl("./sensore", "sensore", NULL)<0){perror("[MAIN]: Errore nella creazione del processo Sensore\n"); exit(1);}
    }else if(pid<0){
        perror("[MAIN]: Errore con il PID nella creazione del processo Sensore\n");
        exit(1);
    }

    pid = fork();

    if(pid == 0) {
       // TODO: processo figlio smistatore (eseguibile distinto)
       printf("[MAIN]: Creazione del Processo Smistatore...\n"); 
       if(execl("./smistatore", "smistatore", NULL)<0){perror("[MAIN]: Errore nella creazione del processo Smistatore\n"); exit(1);}
       	
    }else if(pid<0){
        perror("[MAIN]: Errore con il PID nella creazione del processo Smostatore\n");
        exit(1);
    }


    // TODO: attendere completamento dei figli
    for(int k=0; k<2; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);
    }
    
    // TODO: rimuovere la coda
    msgctl(id_coda_sensore, IPC_RMID, NULL);
        
    return 0;

}
