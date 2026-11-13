#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "header.h"

int main() {

    int shm_id = shmget(IPC_PRIVATE, sizeof(MonitorCoda), IPC_CREAT|0664);/* TBD: Ottenere un descrittore della shared memory */
    
    if (shm_id < 0)
    {
        perror("Errore creazione shared memory");
        exit(1);
    }
    
    MonitorCoda *p = shmat(shm_id, NULL, 0);/* TBD: Effettuare l'attache della shared memory */
    
    if (p == (void *)-1)
    {
        perror("Errore attach shared memory");
        exit(1);
    }

    /* TBD: Inizializzare monitor e variabili per la sincronizzazione tramite la procedura inizializza(...) */

    inizializza(p);
    
    /* TBD: Aggiungere codice per la creazione dei processi produttori e consumatori */
    for(int j=0; j<NUM_PRODUTTORI; j++){

        pid_t pid= fork();

            if(pid==0){
                printf("[MAIN]: Creazione del processo Produttore n. %d\n", j+1);
                produttore(p);
                exit(0);
            }else if(pid<0){
                printf("[MAIN]: Errore nella creazione del processo Produttore\n");
                exit(1);
            }
    }


    for(int j=0; j<NUM_CONSUMATORI; j++){

        pid_t pid2= fork();

            if(pid2==0){
                printf("[MAIN]: Creazione del processo Consumatore n. %d\n", j+1);
                consumatore(p);
                exit(0);
            }else if(pid2<0){
                printf("[MAIN]: Errore nella creazione del processo Consumatore\n");
                exit(1);
            }
    }

   
    /* TBD: Aggiungere codice per l'attesa della terminazione dei processi produttori e consumatori */
    for(int k=0; k<NUM_PRODUTTORI; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo Produttore n. %d\n", k+1);

    }

    for(int k=0; k<NUM_CONSUMATORI; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo Consumatore n. %d\n", k+1);

    }

    
    /* TBD: Aggiungere codice per la rimozione del monitor tramite la procedura rimuovi(...) */
    rimuovi(p);
    shmctl(shm_id, IPC_RMID, NULL);
    
    return 0;
}

