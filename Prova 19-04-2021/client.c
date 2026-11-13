#include "auth.h"

#include <stdio.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


void ClientAuth(int id_coda_auth_req, int id_coda_auth_resp, int id_coda_elab_req, int id_coda_elab_resp) {

    RichiestaAuth msg_auth_req;
    RispostaAuth msg_auth_resp;
    RichiestaElab msg_elab_req;
    RispostaElab msg_elab_resp;

    int ret;


    srand(time(NULL)*getpid());

    for(int i=0; i<5; i++) {


        printf("Client Auth (%d): invio richiesta auth\n", getpid());

        /* TBD: Invio messaggio richiesta nuovo token */
        msg_auth_req.PIDclient= getpid();
        msg_auth_req.token=0;
        msg_auth_req.PIDserver=0;
        msg_auth_req.type= TOKEN_AUTH_REQ_TYPE;

        int result= msgsnd(id_coda_auth_req, (void *)&msg_auth_req, sizeof(RichiestaAuth)-sizeof(long), 0);

        if(result<0){perror("[CLIENTAUTH]: Errore nell'invio della richiesta autorizzazione\n"); exit(1);}

        
        printf("Client Auth (%d): attesa risposta auth\n", getpid());

        /* TBD: Ricezione messaggio con token */
        int result2= msgrcv(id_coda_auth_resp, (void *)&msg_auth_resp, sizeof(RispostaAuth)-sizeof(long), getpid(), 0);

        if(result2<0){perror("[CLIENTAUTH]: Errore nella ricezione della risposta dall'auth\n"); exit(1);}


        int token = msg_auth_resp.token;/* TBD */

        printf("Client Auth (%d): ricevuto token %d\n", getpid(), token);


        int valore = rand() % 11;

        printf("Client Auth (%d): invio richiesta elaborazione %d\n", getpid(), valore);

        /* TBD: Invio richiesta elaborazione */
        msg_elab_req.PID= getpid();
        msg_elab_req.token= token;
        msg_elab_req.type= ELABORAZIONE;
        msg_elab_req.valore= valore;

        int result3= msgsnd(id_coda_elab_req, (void *)&msg_elab_req, sizeof(RichiestaElab)-sizeof(long), 0);

        if(result3<0){perror("[CLIENTAUTH]: Errore nell'invio della richiesta elab al server\n"); exit(1);}


        printf("Client Auth (%d): attesa risposta elaborazione\n", getpid());

        /* TBD: Ricezione risposta elaborazione */
        int result4= msgrcv(id_coda_elab_resp, (void *)&msg_elab_resp, sizeof(RispostaElab)-sizeof(long), ELAB_REQ_TYPE, 0);

        if(result4<0){perror("[CLIENTAUTH]: Errore nella ricezione della risposta di elaborazione dal server\n"); exit(1);}


        if(/* TBD: condizione risposta con esito positivo */msg_elab_resp.esito==1) {

            printf("Client Auth (%d): ricevuta risposta positiva\n", getpid());
        }
        else {
            printf("Client Auth (%d): ricevuta risposta negativa\n", getpid());
        }

    }
}

void ClientNonAuth(int id_coda_elab_req, int id_coda_elab_resp) {

    RichiestaElab msg_elab_req;
    RispostaElab msg_elab_resp;

    int ret;


    srand(time(NULL)*getpid());

    for(int i=0; i<5; i++) {

        int token = rand() % 10000; /* Token casuale non valido */
        int valore = rand() % 11;

        printf("Client Non-Auth (%d): invio richiesta elaborazione %d\n", getpid(), valore);

        /* TBD: Invio richiesta elaborazione */
        msg_elab_req.token= token;
        msg_elab_req.valore= valore;
        msg_elab_req.PID= getpid();
        msg_elab_req.type= ELABORAZIONE;


        int result= msgsnd(id_coda_elab_req, (void *)&msg_elab_req, sizeof(RichiestaElab)-sizeof(long), 0);

        if(result<0){perror("[CLIENT_NON_AUTH]: Errore nell'invio della richiesta di elaborazione\n"); exit(1);}


        printf("Client Non-Auth (%d): attesa risposta elaborazione\n", getpid());

        /* TBD: Ricezione risposta elaborazione */
        int result2= msgrcv(id_coda_elab_resp, (void *)&msg_elab_resp, sizeof(RispostaElab)-sizeof(long), ELAB_REQ_TYPE, 0);

        if(result2<0){perror("[CLIENT_NON_AUTH]: Errore nella ricezione della risposta di elaborazione\n"); exit(1);}

        if(/* TBD: condizione risposta con esito positivo */msg_elab_resp.esito==1) {

            printf("Client Non-Auth (%d): ricevuta risposta positiva\n", getpid());
        }
        else {
            printf("Client Non-Auth (%d): ricevuta risposta negativa\n", getpid());
        }

    }
}
