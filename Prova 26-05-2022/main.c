#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "prodcons.h"

void Produttore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b);
void Consumatore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b);

int main() {

    int id_shm1= shmget(IPC_PRIVATE, sizeof(MonitorVettoreCircolare), IPC_CREAT|0664);
    MonitorVettoreCircolare * p = shmat(id_shm1, NULL, 0);/* TBD: Allocare l'oggetto MonitorVettoreCircolare in memoria condivisa */

    if(p==(void*)-1){perror("[MAIN]: Errore nell'attach del vettore circolare\n");exit(1);}

    init_monitor_circolare(p);



    int id_shm2= shmget(IPC_PRIVATE, sizeof(MonitorBufferSingolo), IPC_CREAT|0664);
    MonitorBufferSingolo * b = shmat(id_shm2, NULL, 0);/* TBD: Allocare l'oggetto MonitorBufferSingolo in memoria condivisa */


    if(b==(void*)-1){perror("[MAIN]: Errore nell'attach del buffer\n");exit(1);}

    init_monitor_buffer_singolo(b);


    /* TBD: Creare un processo figlio, che esegua la funzione "Produttore()" */
    pid_t pid= fork();
    if(pid==0){
        printf("[MAIN]: Creazione del processo Produttore...\n");
        Produttore(p, b);
        exit(0);
    }else if(pid<0){
        printf("[MAIN]: Errore nella creazione del processo figlio Produttore\n");
        exit(1);
    }


    /* TBD: Creare un processo figlio, che esegua la funzione "Consumatore()" */
    pid_t pid2= fork();
    if(pid2==0){
        printf("[MAIN]: Creazione del processo Consumatore...\n");
        Consumatore(p, b);
        exit(0);
    }else if(pid2<0){
        printf("[MAIN]: Errore nella creazione del processo figlio Produttore\n");
        exit(1);
    }


    /* TBD: Attenere la terminazione dei processi figli */
    for(int k=0; k<2; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);
    }

    remove_monitor_circolare(p);
    remove_monitor_buffer_singolo(b);

    /* TBD: Deallocare gli oggetti monitor */
    shmctl(id_shm1, IPC_RMID, NULL);
    shmctl(id_shm2, IPC_RMID, NULL);
}


void Produttore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b) {

    int numero_elementi = rand() % 10;

    printf("[PRODUTTORE] Totale elementi da produrre: %d\n", numero_elementi);

    /* TBD: Effettuare una produzione su MonitorBufferSingolo con
            la funzione "produzione_buffer_singolo", inserendo "numero_elementi"
     */
     produzione_buffer_singolo(b, numero_elementi);


    for(int i=0; i<numero_elementi; i++) {
    
        sleep(1);

        int val = rand() % 10;

        printf("[PRODUTTORE] Prodotto: %d\n", val);

        /* TBD: Chiamare la funzione "produzione_circolare" per produrre
                il valore "val"
         */
         produzione_circolare(p, val);
    }
}

void Consumatore(MonitorVettoreCircolare *p, MonitorBufferSingolo *b) {

    /* TBD: Effettuare una consumazione su MonitorBufferSingolo con
            la funzione "consumazione_buffer_singolo", prelevando "numero_elementi"
     */


    int numero_elementi = consumazione_buffer_singolo(b);/* TBD */

    printf("[CONSUMATORE] Totale elementi da consumare: %d\n", numero_elementi);


    for(int i=0; i<numero_elementi; i++) {

        /* TBD: Chiamare la funzione "consumazione_circolare" per prelevare
                un valore e copiarlo in "val"
         */

        int val = consumazione_circolare(p);/* TBD */

        printf("[CONSUMATORE] Consumato: %d\n", val);
    }
}
