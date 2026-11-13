#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

#include "prodcons.h"

void Produttore(MonitorBufferSingolo* b,int id_coda);
void Consumatore(MonitorBufferSingolo* b,int id_coda);

int main() {
	/* TBD: Allocare l'oggetto MonitorBufferSingolo in memoria condivisa */;
    int id_shm= shmget(IPC_PRIVATE, sizeof(MonitorBufferSingolo)-sizeof(long), IPC_CREAT|0664);
    MonitorBufferSingolo* p;
    
    if(id_shm<0){
        printf("[WARNING_MAIN]: Errore nella creazione della shared memory\n");
        exit(1);
    }

    p= shmat(id_shm, NULL, 0);

    if(p==(void*)-1){

        printf("[WARING_MAIN]: Errore durante l'attach della risorsa condivisa\n");
    }

    /* TBD: Inizializzare l'oggetto MonitorBufferSingolo */
    init_monitor_buffer_singolo(p);


    int id_coda = /* TBD: Allocare una coda di messaggi */msgget(IPC_PRIVATE, IPC_CREAT|0664);

    if(id_coda<0){
        printf("[WARNING_MAIN]: Errore nella creazione di una coda di messaggi\n");
        exit(1);
    }

	int pid;
    
    /* TBD: Creare un processo figlio, che esegua la funzione "Consumatore()" */
    pid=fork();

    if(pid==0){
        printf("[MAIN]: Creazione di un processo figlio Consumatore\n");
        Consumatore(p, id_coda);
        exit(0);
    }else if(pid<0){
        printf("[WARNING_MAIN]: Errore nella creazione di un processo Consumatore\n");
        exit(1);
    }

    /* TBD: Creare un processo figlio, che esegua la funzione "Produttore()" */
    
    pid=fork();

    if(pid==0){
        printf("[MAIN]: Creazione di un processo figlio Consumatore\n");
        Produttore(p, id_coda);
        exit(0);
    }else if(pid<0){
        printf("[WARNING_MAIN]: Errore nella creazione di un processo Produttore\n");
        exit(1);
    }



    /* TBD: Attendere la terminazione dei processi figli */
    for(int k=0; k<2; k++){
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);
        wait(NULL);
    }

    /* TBD: Deallocare l'oggetto monitor */
    remove_monitor_buffer_singolo(p);

    /* TBD: Deallocare la coda di messaggi */
    shmctl(id_shm, IPC_RMID, 0);
}


void Produttore(MonitorBufferSingolo* b,int id_coda) {

    int numero_elementi = rand() % 10;

    printf("[%d][PRODUTTORE] Totale elementi da inviare: %d\n",getpid(),numero_elementi);

    /* TBD: Effettuare una produzione su MonitorBufferSingolo con
            la funzione "produzione_buffer_singolo", inserendo "numero_elementi"
     */
    
        produzione_buffer_singolo(b, numero_elementi);


    for(int i=0; i<numero_elementi; i++) {
    
        sleep(1);

        int val = rand() % 10;

        printf("[%d] [PRODUTTORE] Invio del valore: %d\n", getpid(),val);

		messaggio m;
		m.tipo = MESSAGGIO;
		m.valore = val;
        /* TBD: Inviare il valore "val" tramite la coda di messaggi */
        int result= msgsnd(id_coda, (void*)&m, sizeof(messaggio)-sizeof(long), 0);

        if(result<0){perror("[WARNING_PRODUTTORE]: Errore nell'invio di un messaggio\n");}

    }
}

void Consumatore(MonitorBufferSingolo* b,int id_coda) {

    /* TBD: Effettuare una consumazione su MonitorBufferSingolo con
            la funzione "consumazione_buffer_singolo", prelevando "numero_elementi"
     */
        int numero_elementi= consumazione_buffer_singolo(b);
    
    messaggio m;

    for(int i=0; i<numero_elementi; i++) {

        /* TBD: Ricevere il valore "val" tramite la coda di messaggi */

        int result= msgrcv(id_coda, (void*)&m, sizeof(messaggio)-sizeof(long), MESSAGGIO, 0);

        if(result<0){perror("[WARTNING_CONSUMATORE]: Errore nella ricezione di un messaggio\n");}

        int val =  /* TBD */m.valore;

        printf("[%d][CONSUMATORE] Ricezione del valore: %d\n", getpid(),val);
    }
}
