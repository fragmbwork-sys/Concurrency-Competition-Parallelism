#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "auth.h"

void Server(int id_coda_elab_req, int id_coda_elab_resp, int id_coda_auth_req, int id_coda_auth_resp) {

    RichiestaAuth msg_auth_req;
    RispostaAuth msg_auth_resp;
    RichiestaElab msg_elab_req;
    RispostaElab msg_elab_resp;

    int ret;

    int risorsa = 0;


    for(int i=0; i<10; i++) {

        printf("Server: in attesa di richieste\n");

        /* TBD: Ricezione messaggi di richiesta elaborazione */
        int result= msgrcv(id_coda_elab_req, (void *)&msg_elab_req, sizeof(RichiestaElab)-sizeof(long), ELABORAZIONE, 0);

        if(result<0){perror("[SERVER]: Errore nella ricezione di un messaggio richiesta elaborazione dal Client\n"); exit(1);}
        
        
        int valore = msg_elab_req.valore;/* TBD */
        int PIDclient = msg_elab_req.PID;/* TBD */
        int token = msg_elab_req.token;/* TBD */

        printf("Server: ricevuta richiesta %d da %d (token=%d)\n", valore, PIDclient, token);


        printf("Server: invio conferma autenticazione\n");

        /* TBD: Invio messaggio di richiesta conferma autenticazione */
        msg_auth_req.PIDclient= PIDclient;
        msg_auth_req.token= token;
        msg_auth_req.PIDserver= getpid();
        msg_auth_req.type= ELABORAZIONE;

        int result2= msgsnd(id_coda_auth_req, (void *)&msg_auth_req, sizeof(RichiestaAuth)-sizeof(long), 0);

        if(result2<0){perror("[SERVER]: Errore nell'invio di un messaggio richiesta auth all'auth\n"); exit(1);}


        printf("Server: in attesa di conferma\n");

        /* TBD: Ricezione messaggio di risposta conferma autenticazione */ 
        int result3= msgrcv(id_coda_auth_resp, (void*)&msg_auth_resp, sizeof(RispostaAuth)-sizeof(long), getpid(), 0);

        if(result3<0){perror("[SERVER]: Errore nella ricezione di un messaggio risposta conferma auth\n"); exit(1);}


        int esito;
        esito= msg_auth_resp.esito;

        if(/* TBD: condizione conferma autenticazione positivia */esito==1) {

            printf("Server: autenticazione del client %d riuscita\n", PIDclient);

            risorsa = valore;

            printf("Server: nuovo valore risorsa = %d\n", risorsa);

            esito = 1;
        }
        else {

            printf("Server: autenticazione del client %d fallita\n", PIDclient);
 
            esito = 0;
        }

        printf("Server: invio risposta al client\n");

        /* TBD: Invio messaggio di risposta elaborazione, con esito positivo o negativo */
        msg_auth_resp.esito= esito;
        msg_auth_resp.token= token;
        msg_auth_resp.type= ELAB_REQ_TYPE;

        int result4= msgsnd(id_coda_elab_resp, (void *)&msg_elab_resp, sizeof(RispostaElab)-sizeof(long), 0);

    }

}
