#ifndef _HEADER_
#define _HEADER_

#include "monitor_signal_continue.h"

#define DIM 3

#define MESS_DISP 0
#define SPAZIO_LIB 1
#define LIBERO 4
#define IN_USO 5
#define OCCUPATO 6

typedef struct {

    int buffer[DIM];

    int ultimo_produttore_arrivato;
    int ultimo_produttore_avviato;

    /* TBD: Aggiungere ulteriori variabili per la sincronizzazione */

    int vett_stato[DIM];
    int num_posti_liberi;
    int num_posti_occupati;

    Monitor m;
} MonitorProdConsFIFO;

void inizializza(MonitorProdConsFIFO * p);
void produci(MonitorProdConsFIFO * p, int valore);
int consuma(MonitorProdConsFIFO * p);
void distruggi(MonitorProdConsFIFO * p);

#endif