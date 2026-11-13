#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "header.h"


int main(){

    
     //TODO: completare richiesta shared memory ed inizializzazione
     key_t c_mem=/***/IPC_PRIVATE;
     key_t c_mem2= IPC_PRIVATE;
     
     Info_Volo *volo1, *volo2;
     // ...

     int id_shm1= shmget(c_mem, sizeof(Info_Volo), IPC_CREAT|0664);
     int id_shm2= shmget(c_mem2, sizeof(Info_Volo), IPC_CREAT|0664);

     if(id_shm1<0){
          printf("[MAIN]: Errore nella creazione delle memoria condivisa 1\n");
          exit(1);
     }

     if(id_shm2<0){
          printf("[MAIN]: Errore nella creazione delle memoria condivisa 2\n");
          exit(1);
     }

     volo1= shmat(id_shm1, NULL, 0);

     if(volo1== (void*)-1){
          printf("[MAIN]: Errore nell'attach del volo 1\n");
          exit(1);
     }


     volo2= shmat(id_shm2, NULL, 0);

     if(volo2== (void*)-1){
          printf("[MAIN]: Errore nell'attach del volo 2\n");
          exit(1);
     }

     //TODO inizializzare id_volo pari a 1 per volo1 e pari a 2 per volo2
     volo1->id_volo= 1;
     volo1->gate= 0;
     volo1->num_lettori=0;
     volo2->id_volo= 2;
     volo2->gate= 0;
     volo2->num_lettori= 0;

     //TODO: compleare richiesta semafori ed inizializzazione
     key_t c_sem=/***/IPC_PRIVATE;
     key_t c_sem2= IPC_PRIVATE;// ...
     int id_Sem= semget(c_sem, 2, IPC_CREAT|0664);
     int id_Sem2= semget(c_sem2, 2, IPC_CREAT|0664);

     if(id_Sem<0){
          printf("[MAIN]: Errore nella creazione dei semafori 1\n");
          exit(1);
     }

     if(id_Sem2<0){
          printf("[MAIN]: Errore nella creazione dei semafori 2\n");
          exit(1);
     }

     semctl(id_Sem, MUTEX_L, SETVAL, 1);
     semctl(id_Sem, SYNCH, SETVAL, 1);

     semctl(id_Sem2, MUTEX_L, SETVAL, 1);
     semctl(id_Sem2, SYNCH, SETVAL, 1);

     volo1->id_sem= id_Sem;
     volo2->id_sem= id_Sem2;

     //TODO: compleatare richiesta coda messaggi
     key_t c_coda=/***/ftok("./", 'f');
     // ...
     int id_mess= msgget(c_coda, IPC_CREAT|0664);

     if(id_mess<0){
          printf("[MAIN]: Errore nella creazione della coda dei messaggi\n");
          exit(1);
     }


     //TODO generazione tramite fork di scrittore e 4 lettori, 2 per il volo 1 e 2 per il volo 2
     pid_t pid= fork();
     if(pid==0){
          printf("[MAIN]: Creazione dello Scrittore...\n");
          Scrittore(volo1, volo2);
          exit(0);
     }else if(pid<0){
          printf("[MAIN]: Errore nella creazione del processo Scrittore\n");
          exit(1);
     }

     for(int j=0; j<4; j++){
          pid_t pid= fork();
          if(pid==0){
               if(j%2==0){
                    printf("[MAIN]: Creazione del processo Lettore 1...\n");
                    Lettore(volo1, id_mess);
                    exit(0);
               }else{
                    printf("[MAIN]: Creazione del processo Lettore 2...\n");
                    Lettore(volo2, id_mess);
                    exit(0);
               }
          }else if(pid<0){
               printf("[MAIN]: Errore nella creazione dei processi Lettori\n");
               exit(1);
          }
     }
    
     //TODO generazione tramite fork/exec del visualizzatore
     pid_t pid2= fork();
     if(pid2==0){
          if(execl("./visualizzatore", "visualizzatore", NULL)<0){
               perror("[MAIN]: Errore nella creazione del processo Visualizzatore\n");
               exit(1);
          }
     }

     //TODO: attesa terminazione dei "numprocessi"  figli
     int numprocessi = NUM_LETTORI+2;
     for(int k=0; k<numprocessi; k++){
          wait(NULL);
          printf("[MAIN]: Processo n. %d terminato\n", k+1);
     }
    

     //TODO: Deallocazione risorse
     semctl(id_Sem, 0, IPC_RMID);
     semctl(id_Sem2, 0, IPC_RMID);
     msgctl(id_mess, IPC_RMID, NULL);
     shmctl(id_shm1, IPC_RMID, NULL);
     shmctl(id_shm2, IPC_RMID, NULL);
    
     return 0;
}
