#ifndef _HEADER_MSG_
#define _HEADER_MSG_

#define MSG_TYPE 1

typedef struct {

    /* TBD: Definire il messaggio di richiesta */
    long tipo;

    int pid;
    int valori;
    

} msg_init_request;

typedef struct {

    /* TBD: Definire il messaggio di risposta */
    long tipo_risp;

    int shm_invio;
    int sem_invio;

    int shm_ricezione;
    int sem_ricezione;


} msg_init_response;


#endif
