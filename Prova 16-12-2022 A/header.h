/*----HEADER FILE----*/
#ifndef __HEADER
#define __HEADER

#include "semafori.h"

#define DIM 4

// TODO: definire semafori
#define MESS1 0
#define MESS2 1
#define SPAZIO_DISP 2
#define MUTEX_P 3
#define MUTEX_C 4

#define NPROD 4
#define NCONS 2

#define PRODS 4
#define CONS 8

typedef struct {
	
	// TODO: completare campi struct
	int stato[DIM];
	int vettore[DIM];


} BufferMutuaEx;

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO_1 2
#define OCCUPATO_2 3

void produci_tipo_1(BufferMutuaEx * buf, int semid, int val);
void produci_tipo_2(BufferMutuaEx * buf, int semid, int val);
void consuma_tipo_1(BufferMutuaEx * buf, int semid);
void consuma_tipo_2(BufferMutuaEx * buf, int semid);



#endif
