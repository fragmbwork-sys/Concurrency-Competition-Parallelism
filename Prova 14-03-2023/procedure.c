#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"


/*******PROCEDURE PER I SEMAFORI******/


void Wait_Sem(int id_sem, int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;
       semop(id_sem,&sem_buf,1);   //semaforo rosso
}


  void Signal_Sem (int id_sem,int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;
       semop(id_sem,&sem_buf,1);   //semaforo verde
}


/***********************************************/

void Lettore(Info_Volo *volo, int coda){
	int i;
        for (i=0; i<NUM_OPERAZIONI; i++) {

                //TODO: accesso alla risorsa secondo soluzione lettori/scrittori, starvation scrittori
                Wait_Sem(volo->id_sem, MUTEX_L);

                volo->num_lettori++;

                if(volo->num_lettori==1){
                        printf("[LETTORE %d]: Sono il primo lettore, fermo gli scrittori\n", volo->id_volo);
                        Wait_Sem(volo->id_sem, SYNCH);
                }

                Signal_Sem(volo->id_sem, MUTEX_L);

                //TODO: creazione e invio messaggio al visualizzatore
                msg messaggio;
                messaggio.volo= volo->id_volo;
                messaggio.gate= volo->gate;

                int result= msgsnd(coda, (void *)&messaggio, sizeof(msg)-sizeof(long), 0);

                if(result<0){perror("[LETTORE]: Errore nell'invio del messaggio\n"); exit(1);}


               
                sleep(1);

               //TODO: rilascio risorsa secondo soluzione lettori/scrittori, starvation scrittori
               Wait_Sem(volo->id_sem, MUTEX_L);

               volo->num_lettori--;

               if(volo->num_lettori==0){
                printf("[LETTORE %d]: Sono l'ultimo lettore, sveglio gli scrittori\n", volo->id_volo);
                Signal_Sem(volo->id_sem, SYNCH);
               }

               Signal_Sem(volo->id_sem, MUTEX_L);

               

        }

}

void Scrittore(Info_Volo* volo1, Info_Volo* volo2) {
        int i;
        for (i=0; i<NUM_OPERAZIONI; i++) {

                //TODO: accesso alle risorse secondo soluzione lettori/scrittori, starvation scrittori
                Wait_Sem(volo1->id_sem, SYNCH);
                Wait_Sem(volo2->id_sem, SYNCH);


                //TODO: gate del volo 2 = gate del volo 1
                volo2->gate= volo1->gate;
                printf("[SCRITTORE]: gate del volo 2: %d = gate del volo 1: %d\n", volo2->gate, volo1->gate);
                //TODO: gate del volo 1 = valore casuale da 1 a 100 
                sleep(1);

                volo1->gate= (rand()%100)+1;
                printf("[SCRITTORE]: gate del volo 1 = valore casuale: %d\n", volo1->gate);
                //TODO: rilascio  risorse secondo soluzione lettori/scrittori, starvation scrittori
                Signal_Sem(volo1->id_sem, SYNCH);
                Signal_Sem(volo2->id_sem, SYNCH); 
        }

}


