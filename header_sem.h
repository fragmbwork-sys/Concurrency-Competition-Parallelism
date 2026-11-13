#ifndef _HEADER_SEM_
#define _HEADER_SEM_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef struct {

    /* TBD: Completare la struttura con le variabili
            necessarie per lo schema lettori-scrittori
     */
     int num_lettori;
     int num_scrittori;

     int buffer;

} lettscritt;

#define SYNCH 1
#define MUTEXL 0


int Wait_Sem(int id_sem, int numsem);
int Signal_Sem(int id_sem, int numsem);

void scrivi(int id_sem, lettscritt * p, int valore);
int leggi(int id_sem, lettscritt * p);

#endif