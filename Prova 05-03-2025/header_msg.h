#ifndef _HEADER_H
#define _HEADER_H

#include <sys/ipc.h>
#include <sys/msg.h>

/* TBD: Completare la definizione delle strutture per i messaggi.
        È possibile definire in questo file delle costanti con la direttiva "define" 
 */

 #define OK_TO_SEND 1
 #define REQUEST_TO_SEND 2
 #define MESSAGGIO 3
 

struct richiesta {

    /* TBD */

    long tipo;

    int mess;
};

struct rts {
    /* TBD */

    long tipo_request;

    int mess_request;
};

struct ots {
    /* TBD */

    long tipo_ok;
    int mess_ok;
};

#endif