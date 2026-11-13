#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <unistd.h>

#include "header.h"

int main() {

    /* TBD: Ottenere i riferimenti alle code di messaggi */
    key_t key_coda1= ftok(".", 'f');
    key_t key_coda2= ftok(".", 'g');
 
    int idmess_req= msgget(key_coda1, 0);
    int idmess_risp= msgget(key_coda2, 0);

    //srand(getpid());

    for(int i=0; i<NUMERO_RICHIESTE; i++) {
        

        int tipo_richiesta;
        int valore;
        int posizione = rand() % DIM_VETTORE;

        if((rand() % 2) == 0) {

            tipo_richiesta = /* TBD */1; //LETTURA
            valore = 0;
        }
        else {
            
            tipo_richiesta = /* TBD */2; //SCRITTURA
            valore = rand() % 10;
        }

        printf("[CLIENT %d] Invio richiesta (tipo=%d, valore=%d, posizione=%d)\n", getpid(), tipo_richiesta, valore, posizione);

        /* TBD: Inviare un messaggio di richiesta */
        messaggio_richiesta richiesta;
        richiesta.tipo= tipo_richiesta;
        richiesta.identificativo= getpid();
        richiesta.valoreM= valore;
        richiesta.posizione= posizione;

        int result= msgsnd(idmess_req, (void*)&richiesta, sizeof(messaggio_richiesta)-sizeof(long), 0);

        if(result<0){perror("[WARNING_CLIENT]: Errore nell'invio di un messaggio\n");}

        
        /* TBD: Ricevere il messaggio di risposta */
        messaggio_risposta risp;

        result= msgrcv(idmess_risp, (void*)&risp, sizeof(risp)-sizeof(long), getpid(), 0);

        if(result<0){perror("[WARNING_CLIENT]: Errore nella ricezione di un messaggio\n");}

        int valore_risposta = /* TBD */risp.valore_letto;

        printf("[CLIENT %d] Ricezione risposta (valore=%d)\n", getpid(), valore_risposta);
    }


}