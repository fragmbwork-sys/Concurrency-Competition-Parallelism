#include "monitor_hoare.h"
#include "semafori.h"

#define DIM 6
#define TOT_GIOCATTOLI 20

/*
    TODO: definire quanto necessario per la gestione del MonitorProdCons con vettore di stato 
*/
#define LIBERO 0
#define IN_USO 1
#define OCCUPATO_GIOCO1 2
#define OCCUPATO_GIOCO2 3

#define COND_PROD 0
#define COND_CONS1 1
#define COND_CONS2 2


/*
    TODO: definire quanto necessario per la gestione del buffer mediante utilizzo di semafori
*/
#define SPAZIO_DISP 0
#define MUTEX 1
#define MESS_DISP 2



typedef struct {
    /*
        TODO: completare struttura
    */
    int nelem_liberi;
    int nelem_occupati;
    int nelem_tipo1;
    int nelem_tipo2;
    int stato[DIM];
    int vettore[DIM];

    Monitor m;

} MonitorProdCons;

typedef struct {
    /*
        TODO: completare struttura
    */
    int semid;
    int buffer;

} BufferBabbo;

void inizializza_vettore(MonitorProdCons *pc);
void rimuovi_vettore(MonitorProdCons *pc);
void inizializza_buffer(BufferBabbo * bb);
void rimuovi_buffer(BufferBabbo * bb);

void Produttore(MonitorProdCons *pc);
void produci(MonitorProdCons *pc, int tipo);
void Consumatore1(MonitorProdCons *pc, BufferBabbo *bb);
void Consumatore2(MonitorProdCons *pc, BufferBabbo *bb);
void BabboNatale(BufferBabbo *bb);