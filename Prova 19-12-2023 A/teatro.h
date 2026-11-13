#include <pthread.h>

#ifndef HEADER_H
#define HEADER_H

/* TODO: Definire (se necessario) le macro di utilità */

typedef struct{

    int ID;
    int tipo;
    
} posto_teatro;

typedef struct {
    
    /* TODO: Aggiungere le variabili per la sincronizzazione */
    int num_lett;
    int num_scritt;
    int num_scritt_wait;
    int num_lett_wait;

    pthread_cond_t VAR_LETT;
    pthread_cond_t VAR_SCRITT;

    pthread_mutex_t mutex;
    
    posto_teatro *p;
    
} LettScritt;

typedef struct {

    long msg_type;
    
    int tipo;
    int value;
    
} Msg_Posto;

void * clientT(void *);
void * viewerT(void *);

void scrivi_posto(LettScritt *ls, posto_teatro *);
void leggi_posto(LettScritt *ls, int);

void InizioLetturaPosto(LettScritt * ls);
void FineLetturaPosto(LettScritt * ls);
void FineScritturaPosto(LettScritt * ls);
void InizioScritturaPosto(LettScritt * ls);


#endif
