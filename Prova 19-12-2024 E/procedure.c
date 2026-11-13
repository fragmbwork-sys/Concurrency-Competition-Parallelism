#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"

void inizializza_coda(MonitorPC *pc) {
/*
    TODO: completare
*/
    pc->testa=0;
    pc->coda=0;
    //pc->id_coda= msgget(IPC_PRIVATE, IPC_CREAT|0665);
    init_monitor(&(pc->m), 2);
}

void rimuovi_coda(MonitorPC *pc) {
/*
    TODO: completare
    
*/

     remove_monitor(&(pc->m));
}

void produci(MonitorPC *pc, Lettera lettera) {
/*
    TODO: completare produzione con coda circolare
*/

    enter_monitor(&(pc->m));

    printf("[PRODUCI_PROCEDURE]: Un produttore e' entrato nel monitor.\n");

    while((pc->testa+1)%DIM==pc->coda){
        wait_condition(&(pc->m), SPAZIO_DISP);
    }

    printf("[PRODUCI_PROCEDURE]: Un produttore ha superato il while.\n");

    pc->buffer[pc->coda].id= lettera.id;
    pc->buffer[pc->coda].num_regali= lettera.num_regali;
    pc->coda= (pc->coda+1)%DIM;

    
    signal_condition(&(pc->m), MESS_DISP);

    leave_monitor(&(pc->m));


    printf("[Produttore %d] Ho inviato lettera: ID=%ld, regali=%d\n", getpid(), lettera.id, lettera.num_regali);

}

Lettera consuma(MonitorPC *pc) {
    Lettera lettera;
/*
    TODO: completare consumo con coda circolare
*/

    enter_monitor(&(pc->m));

    printf("[CONSUMA_PROCEDURE]: Consumatore entra nel monitor\n");

    while(pc->testa==pc->coda){
        wait_condition(&(pc->m), MESS_DISP);
    }

    printf("[CONSUMA_PROCEDURE]: Consumatore esce dalla condizione while\n");

    lettera.id= pc->buffer[pc->testa].id;
    lettera.num_regali= pc->buffer[pc->testa].num_regali;
    pc->testa=(pc->testa+1)%DIM;


    signal_condition(&(pc->m), SPAZIO_DISP);

    leave_monitor(&(pc->m));

    return lettera;
}

void SendSincr (MessaggioRispostaBabbo *mess, int queue){
/*
    TODO: completare
*/
    MessaggioRispostaBabbo m1,m2;

    m1.tipo= RQST_TO_SND;

    int result= msgsnd(queue, (void*)&m1, sizeof(MessaggioRispostaBabbo)-sizeof(long), 0);

    if(result<0){perror("[SENDSINCR_PROCEDURE]: Errore nell'invio della REQUEST TO SEND\n");}

    int result1= msgrcv(queue, (void*)&m2, sizeof(MessaggioRispostaBabbo)-sizeof(long), OK_TO_SND, 0);

    if(result1<0){perror("[SENDSINCR_PROCEDURE]: Errore nella receive della OK TO SEND\n");}

    int result2= msgsnd(queue, (void*)mess, sizeof(MessaggioRispostaBabbo)-sizeof(long), 0);

    if(result2<0){perror("[SENDSINCR_PROCEDURE]: Errore nell'invio della risposta di Babbo Natale\n");}

}

void ReceiveBloc(MessaggioRispostaBabbo *mess, int queue){
/*
    TODO: completare
*/
    MessaggioRispostaBabbo m1, m2;
    m2.tipo= OK_TO_SND;

    int result= msgrcv(queue, (void*)&m1, sizeof(MessaggioRispostaBabbo)-sizeof(long), RQST_TO_SND, 0);

    if(result<0){perror("[RECEIVEBLOC_PROCEDURE]: Errore nella receive della REQUEST TO SEND\n");}

    int result1= msgsnd(queue, (void*)&m2, sizeof(MessaggioRispostaBabbo)-sizeof(long), 0);

    if(result1<0){perror("[RECEIVEBLOC_PROCEDURE]: Errore nell'invio della OK TO SEND\n");}

    int result2= msgrcv(queue, (void*)mess, sizeof(MessaggioRispostaBabbo)-sizeof(long), RISPOSTA_BABBO, 0);

    if(result2<0){perror("[RECEIVEBLOC_PROCEDURE]: Errore nella receive della risposta di Babbo Natale\n");}

}

void Produttore(MonitorPC *pc, int id_bambino, int coda_risposte) {
    /*
        TODO: completare inizializzazione lettera e produzione
    */
    Lettera lettera;

        lettera.num_regali= (rand()%5)+1;
        lettera.id= id_bambino;

        produci(pc, lettera);

    /*
        TODO: attesa risposta
    */
    MessaggioRispostaBabbo risposta;

   ReceiveBloc(&risposta,coda_risposte);

    printf("[Produttore %d] Risposta ricevuta: %s\n", getpid(), risposta.messaggio);
}

void Consumatore(MonitorPC *pc, int coda_risposte) {
    int cattivi[] = {3, 5, 7, 9};
    int num_cattivi = sizeof(cattivi) / sizeof(cattivi[0]);

    for (int i=0; i<NUM_BAMBINI; i++) {
        /*
            TODO: completare consumo di una lettera
        */
       Lettera lettera= consuma(pc);

        printf("[Babbo Natale] Ho ricevuto una lettera con ID=%ld e regali=%d\n", lettera.id, lettera.num_regali);

        int id_bambino= lettera.id;

        /*
            TODO: creazione messaggio di risposta, controllo dell'id e invio del messaggio di risposta
        */
       MessaggioRispostaBabbo risposta;
       risposta.tipo= RISPOSTA_BABBO;
       controlla_buono(&risposta, id_bambino, cattivi);

       SendSincr(&risposta, coda_risposte);

        printf("[Babbo Natale] Il bambino è stato %s\n", risposta.messaggio);

        sleep(1);
    }
}

void controlla_buono(MessaggioRispostaBabbo *risposta, int id, int *cattivi) {
    strcpy(risposta->messaggio, "buono");
    for (int i = 0; i < NUM_CATTIVI; i++) {
        if (cattivi[i] == id) {
            strcpy(risposta->messaggio, "cattivo");
            break; 
        }
    }
}

