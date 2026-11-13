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



    for(int i=0; i<NUMERO_RICHIESTE; i++) {

        /* TBD: Ricevere un messaggio di richiesta dal client */
        messaggio_richiesta richiesta;

        int result= msgrcv(idmess_req, (void*)&richiesta, sizeof(messaggio_richiesta)-sizeof(long), 0, 0);

        if(result<0){perror("[WARNING_SERVER]: Errore nella ricezione di un messaggio\n");}

        int tipo_richiesta = /* TBD */richiesta.tipo;
        int valore = /* TBD */richiesta.valoreM;
        int posizione = /* TBD */richiesta.posizione;
        int pid_client = /* TBD */richiesta.identificativo;

        printf("[SERVER %d] Inoltro richiesta (tipo=%d, pid=%d, posizione=%d, valore=%d)\n", getpid(), tipo_richiesta, pid_client, posizione, valore);

        sleep(1);


        /* TBD: Inviare un messaggio di richiesta al database, contenente lo stesso tipo di richiesta (LETTURA/SCRITTURA), posizione, e valore */

        messaggio_richiesta richiesta_server;
        richiesta_server.tipo= tipo_richiesta;
        richiesta_server.identificativo= getpid();
        richiesta_server.valoreM= valore;
        richiesta_server.posizione= posizione;

        result= msgsnd(idmess_req, (void*)&richiesta_server, sizeof(messaggio_richiesta)-sizeof(long), 0);

        if(result<0){perror("[WARNING_SERVER]: Errore nell'invio di un messaggio al DATABASE\n");}


        /* TBD: Ricevere un messaggio di risposta dal database */
        messaggio_risposta risp;

        result= msgrcv(idmess_risp, (void*)&(risp), sizeof(messaggio_risposta)-sizeof(long), getpid(), 0);


        if(result<0){perror("[WARNING_SERVER]: Errore nella ricezione di un messaggio dal DATABASE\n");}

        
        int valore_risposta = /* TBD */risp.valore_letto;

        printf("[SERVER %d] Inoltro risposta (pid=%d, valore=%d)\n", getpid(), pid_client, valore_risposta);



        /* TBD: Inviare il messaggio di risposta al client, contenente lo stesso valore risposta */
        messaggio_risposta risp_al_client;
        risp_al_client.tipo_risp= pid_client;
        risp.valore_letto= valore_risposta;

        result= msgsnd(idmess_risp, (void*)&risp_al_client, sizeof(messaggio_risposta)-sizeof(long), 0);

        if(result<0){perror("[WARNING_SERVER]: Errore nell'invio di un messaggio al CLIENT\n");}



    }

}