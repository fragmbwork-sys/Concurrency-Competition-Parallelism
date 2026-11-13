#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "header_msg.h"

int main() {

    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t chiave_ots= ftok(".", 'f');
    key_t chiave_rts= ftok(".", 'g');
    key_t chiave_main= ftok(".", 'h');
    int id_msg_ots= msgget(chiave_ots, 0);
    int id_msg_rts= msgget(chiave_rts, 0);
    int id_msg_main= msgget(chiave_main, 0);


    srand(getpid());

    for(int i=0; i<3; i++) {

        struct richiesta r;
        struct rts rts;
        struct ots ots;

        /* TBD: Inviare il messaggio "request to send" */
        rts.tipo_request= REQUEST_TO_SEND;
        rts.mess_request= 0;

        int result= msgsnd(id_msg_rts, (void *)&rts, sizeof(struct rts)-sizeof(long), 0);

        if(result<0){
            printf("[WARNING_CLIENT]: Errore nell'invio del messaggio REQUEST_TO_SEND\n");
            exit(1);
        }else{

        printf("[CLIENT] Inviato messaggio RTS\n");

        }

        
        /* TBD: Ricevere il messaggio "ok to send" */
        result= msgrcv(id_msg_ots, (void *)&ots, sizeof(struct rts)-sizeof(long), OK_TO_SEND, 0);

        if(result<0){
            printf("[WARNING_CLIENT]: Errore nella ricezione del messaggio OK_TO_SEND\n");
            exit(1);
        }else{

        printf("[CLIENT] Ricevuto messaggio OTS\n");
        
        }

        int valore = rand() % 10;

        /* TBD: Inviare un messaggio con il valore */
        r.mess= valore;
        r.tipo= MESSAGGIO;

        result= msgsnd(id_msg_main, (void *)&r, sizeof(struct richiesta)-sizeof(long), 0);

        if(result<0){
            printf("[WARNING_CLIENT]: Errore nell'invio del messaggio\n");
            exit(1);
        }else{

        printf("[CLIENT] Inviato messaggio MSG con valore %d\n", valore);
        }

    }
}