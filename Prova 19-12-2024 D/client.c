#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "header.h"


int main() {
    /*
        TODO: recupero code
    */

    int key_richieste= ftok("./", 'a');
    int coda_richieste= msgget(key_richieste, 0);

    int key_risposte= ftok("./", 'z');
    int coda_risposte= msgget(key_risposte, 0);

    if (coda_richieste == -1 || coda_risposte == -1) {
        perror("Errore nell'apertura delle code");
        exit(1);
    }

    for (int i = 0; i < NUM_BAMBINI; i++) {
        /* TODO */
        pid_t pid= fork();
            if(pid==0){
                printf("[CLIENT]: Creazione del processo bambino n. %d\n", i+1);
                exit(0);
            }else if(pid<0){
                printf("[CLIENT]: Errore nella creazione del processo bambino n. %d\n", i+1);
                exit(1);
            }

            //srand(time(NULL)*getpid());
            for (int j = 0; j < NUM_LETTERE; j++) {
                /*
                    TODO: costruzione e invio del messaggio
                */
               MessaggioRichiesta msg;
               msg.tipo_ric= RICHIESTA;

                /* TODO */msg.numero_regali = (rand() % 5) + 1;

                printf("Invio lettera a Babbo Natale chiedendo %d regali\n", msg.numero_regali);

                int result= msgsnd(coda_richieste, (void*)&msg, sizeof(MessaggioRichiesta)-sizeof(long), 0);

                if(result<0){perror("[CLIENT]: Errore nell'invio di un messaggio di richiesta\n");}


                /*
                    TODO: attesa del messaggio di risposta
                */

                MessaggioRisposta risposta;

                result= msgrcv(coda_risposte, (void*)&risposta, sizeof(MessaggioRisposta)-sizeof(long), RISPOSTA, 0);

                if(result<0){perror("[CLIENT]: Errore nella ricezione di un messaggio di risposta\n");}


                printf("[Bambino %d] Lettera %d: conferma ricevuta\n", i + 1, j + 1);
            }
            //exit(0);
        }
    

    /*
        TODO: Attesa della terminazione dei processi figli
    */

    for(int k=0; k<NUM_BAMBINI; k++){
        wait(NULL);
        printf("[CLIENT]: Processo bambino n. %d terminato.\n", k+1);
    }

    return 0;
}
