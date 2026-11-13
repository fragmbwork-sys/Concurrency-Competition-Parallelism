#include "auth.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

typedef struct {
    pid_t pid;
    int token;
} autenticazione;


void Auth(int id_coda_auth_req, int id_coda_auth_resp) {

    RichiestaAuth msg_auth_req;
    RispostaAuth msg_auth_resp;

    int ret;

    autenticazione vettore_autenticazioni[10];
    int totale_autenticazioni = 0;


    for(int i=0; i<15; i++) {

        printf("Auth: in attesa di richieste\n");

        /* TBD: ricezione richieste */
        int result= msgrcv(id_coda_auth_req, (void *)&msg_auth_req, sizeof(RichiestaAuth)-sizeof(long), MESSAGGIO_CLIENT, 0);

        if(result<0){perror("[AUTH]: Errore nella ricezione di un messaggio di richiesta\n"); exit(1);}


        if(/* TBD: richiesta di nuovo token */msg_auth_req.type== TOKEN_AUTH_REQ_TYPE) {

            int token = rand() % 10000;

            pid_t PIDclient = /* TBD */ msg_auth_req.PIDclient;

            vettore_autenticazioni[totale_autenticazioni].token = token;
            vettore_autenticazioni[totale_autenticazioni].pid = PIDclient;
            totale_autenticazioni++;

            printf("Auth: salvataggio token %d per processo %d\n", token, PIDclient);


            printf("Auth: invio token\n");

            /* TBD: Invio messaggio con token */
            msg_auth_resp.token= token;
            msg_auth_resp.type= PIDclient;

            int result= msgsnd(id_coda_auth_resp, (void *)&msg_auth_resp, sizeof(RispostaAuth)-sizeof(long), 0);

            if(result<0){perror("[AUTH]: Errore nell'invio del messaggio con token al client\n"); exit(1);}

            //msg_auth_resp.esito = 1;
 

        }
        else if(/* TBD: richiesta di conferma autenticazione */msg_auth_req.type== ELAB_REQ_TYPE) {

            // Ricerca token nel vettore autenticazioni
            int trovato = -1;

            pid_t PIDclient = msg_auth_req.PIDclient;/* TBD */
            int token = msg_auth_req.token;/* TBD */ 


            for(int k=0; k<totale_autenticazioni; k++) {

                if(vettore_autenticazioni[k].pid == PIDclient &&
                    vettore_autenticazioni[k].token == token) {

                    printf("Auth: token di autenticazione di %d trovato\n", PIDclient);
                    trovato = k;
                    break;
                }
            }


            printf("Auth: invio messaggio di conferma autenticazione\n");

            if(trovato == -1) {

                /* TBD: Invio messaggio di autenticazione fallita//riuscita */
                msg_auth_resp.type= msg_auth_req.PIDserver;
                msg_auth_resp.esito= 0;
            }
            else {

                /* TBD: Invio messaggio di autenticazione riuscita//fallita */
                msg_auth_resp.type= msg_auth_req.PIDserver;
                msg_auth_resp.esito= 1;
            }



        }
        else {
            printf("Auth: ricevuto messaggio di tipo errato\n");
        }
    }
}
