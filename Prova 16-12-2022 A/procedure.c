/*----IMPLEMENTAZIONE DELLE PROCEDURE----*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "semafori.h"
#include <math.h>

void produci_tipo_1(BufferMutuaEx * buf, int semid, int value) {

	// TODO: completare produzione 1
	Wait_Sem(semid, SPAZIO_DISP);

	Wait_Sem(semid, MUTEX_P);

	int index= 0;

	while(index<DIM && buf->stato[index]!=LIBERO){
		index++;
	}

	buf->stato[index]= IN_USO;
	
	Signal_Sem(semid, MUTEX_P);



	buf->vettore[index]= value;
	printf("[Produttore %d] ho prodotto %d di tipo 1, in posizione %d\n", getpid(), value, index);



	Wait_Sem(semid, MUTEX_P);

	buf->stato[index]= OCCUPATO_1;

	Signal_Sem(semid, MESS1);

	Signal_Sem(semid, MUTEX_P);
	

}

void produci_tipo_2(BufferMutuaEx * buf, int semid, int value) {

	// TODO: completare produzione 2 
	Wait_Sem(semid, SPAZIO_DISP);

	Wait_Sem(semid, MUTEX_P);

	int index= 0;

	while(index<DIM && buf->stato[index]!=LIBERO){
		index++;
	}

	buf->stato[index]= IN_USO;
	
	Signal_Sem(semid, MUTEX_P);



	buf->vettore[index]= value;
	printf("[Produttore %d] ho prodotto %d di tipo 2, in posizione %d\n", getpid(), value, index);



	Wait_Sem(semid, MUTEX_P);

	buf->stato[index]= OCCUPATO_2;

	Signal_Sem(semid, MESS2);

	Signal_Sem(semid, MUTEX_P);


}

void consuma_tipo_1(BufferMutuaEx * buf, int semid) {

	// TODO: completare consumo 1

	Wait_Sem(semid, MESS1);

	Wait_Sem(semid, MUTEX_C);

	int index= 0;

	while(index<DIM && buf->stato[index]!=OCCUPATO_1){
		index++;
	}

	buf->stato[index]= IN_USO;

	Signal_Sem(semid, MUTEX_C);



	int value= buf->vettore[index];
	printf("[Consumatore %d] consumo elemento %d di tipo 1 in posizione %d\n",getpid(), value, index);



	Wait_Sem(semid, MUTEX_C);

	buf->stato[index]= LIBERO;

	Signal_Sem(semid, SPAZIO_DISP);

	Signal_Sem(semid, MUTEX_C);	

}

void consuma_tipo_2(BufferMutuaEx * buf, int semid) {
	
	// TODO: completare consumo 2

	Wait_Sem(semid, MESS2);

	Wait_Sem(semid, MUTEX_C);

	int index= 0;

	while(index<DIM && buf->stato[index]!=OCCUPATO_2){
		index++;
	}

	buf->stato[index]= IN_USO;

	Signal_Sem(semid, MUTEX_C);



	int value= buf->vettore[index];
	printf("[Consumatore %d] consumo elemento %d di tipo 2 in posizione %d\n",getpid(), value, index);	



	Wait_Sem(semid, MUTEX_C);

	buf->stato[index]= LIBERO;

	Signal_Sem(semid, SPAZIO_DISP);

	Signal_Sem(semid, MUTEX_C);

}
