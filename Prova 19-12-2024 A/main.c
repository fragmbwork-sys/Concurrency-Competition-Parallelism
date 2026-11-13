#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "header.h"

#define NUM_CONSUMI 10

int main() {

    /*
        TODO: inizializzazione segmenti di memoria e strutture condivisi
    */
    /*int id_sem= semget(IPC_PRIVATE, 2, IPC_CREAT|0664);

    if(id_sem<0){
        printf("[MAIN]: Errore nella creaizone della coda dei semafori\n");
        exit(1);
    }

    semctl(id_sem, MUTEX, 1);
    semctl(id_sem, SPAZIO_DISP, 1);*/

    int id_shm1= shmget(IPC_PRIVATE, sizeof(MonitorProdCons), IPC_CREAT|0664);

    if(id_shm1<0){
        printf("[MAIN]: Errore nella creazione della memoria condivisa Monitor\n");
        exit(1);
    }

    MonitorProdCons *pc= (MonitorProdCons*)shmat(id_shm1, NULL, 0);

    if(pc==(void*)-1){
        printf("[MAIN]: Errore nell'attach dell'oggetto MonitorProdcons\n");
        exit(1);
    }

    inizializza_vettore(pc);

    int id_shm2= shmget(IPC_PRIVATE, sizeof(BufferBabbo), IPC_CREAT|0664);

    if(id_shm2<0){
        printf("[MAIN]: Errore nella creazione della memoria condivisa Buffer\n");
        exit(1);
    }

    BufferBabbo *bb= (BufferBabbo*)shmat(id_shm2, NULL, 0);

    if(bb==(void*)-1){
        printf("[MAIN]: Errore nell'attach dell'oggetto BufferBabbo\n");
        exit(1);
    }

    inizializza_buffer(bb);


    pid_t pid_p, pid_c1, pid_c2, pid_b;

    /*
        TODO: creazione di 4 figli: un Produttore, un Consumatore1, un Consumatore2 e BabboNatale
    */

    
    pid_p= fork();
    if(pid_p==0){
        printf("[MAIN]: Creazione del processo Produttore...\n");
        Produttore(pc);
        exit(0);
    }else if(pid_p<0){
        printf("[MAIN]: Errore nella creazione del processo Produttore\n");
        exit(1);
    }


    pid_c1= fork();
    if(pid_c1==0){
        printf("[MAIN]: Creazione del processo Consumatore 1...\n");
        Consumatore1(pc, bb);
        exit(0);
    }else if(pid_c1<0){
        printf("[MAIN]: Errore nella creazione del processo Consumatore 1\n");
        exit(1);
    }


    pid_c2= fork();
    if(pid_c2==0){
        printf("[MAIN]: Creazione del processo Consumatore 2...\n");
        Consumatore2(pc, bb);
        exit(0);
    }else if(pid_c2<0){
        printf("[MAIN]: Errore nella creazione del processo Consumatore 2\n");
        exit(1);
    }


    pid_b= fork();
    if(pid_b==0){
        printf("[MAIN]: Creazione del processo Babbo Natale...\n");
        BabboNatale(bb);
        exit(0);
    }else if(pid_b<0){
        printf("[MAIN]: Errore nella creazione del processo Babbo Natale\n");
        exit(1);
    }

    /*
        TODO: attesa terminazione figli
    */
    for(int k=0; k<4; k++){

        wait(NULL);
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);
    }

    /*
        TODO: distruzione strutture
    */
    rimuovi_vettore(pc);
    rimuovi_buffer(bb);
    //semctl(id_sem, 0, IPC_RMID);

    shmctl(id_shm1, IPC_RMID, NULL);
    shmctl(id_shm2, IPC_RMID, NULL);

    

    return 0;
}