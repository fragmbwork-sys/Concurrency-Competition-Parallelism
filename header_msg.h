#ifndef _HEADER_MSG_
#define _HEADER_MSG_

/* TBD: Definire le strutture dati per l'invio dei messaggi */

#define TIPO_MESS 1
#define REQUEST_TO_SEND 2
#define OK_TO_SEND 3

typedef struct{

    long tipo;
    int valore;
    int buffer_destinazione;

}MessaggioClient;

#define NUM_BUFFER 3
#define NUM_SERVER (NUM_BUFFER*2)
#define NUM_RICHIESTE 4

#endif