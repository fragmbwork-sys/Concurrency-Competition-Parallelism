#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <unistd.h>
#include "header.h"

typedef struct {
    MonitorLettScritt * ls;
    int regali_richiesti;
} ParametriScrittore;

int coda_richieste;
int coda_risposte;

void inizializza_struttura(MonitorLettScritt * ls) {
    /*
        TODO: completare
    */
    ls->num_lettori= 0;
    ls->num_scritt= 0;
    ls->num_scrittori_wait= 0;

    ls->totale_regali= 0;
    
    pthread_mutex_init(&ls->mutex, NULL);
    pthread_cond_init(&ls->VAR_SCRITT, NULL);
    pthread_cond_init(&ls->VAR_LETT, NULL);
}

void distruggi_struttura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_destroy(&ls->mutex);
    pthread_cond_destroy(&ls->VAR_SCRITT);
    pthread_cond_destroy(&ls->VAR_LETT);
}

void inizio_lettura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_lock(&ls->mutex);

    while(ls->num_scritt>0){
        printf("[LETTURA]: Non posso ancora leggere il dato, ci sono gia' scrittori \n");
        pthread_cond_wait(&ls->VAR_LETT, &ls->mutex);
    }

    printf("[LETTURA]: Sono uscito dalla wait\n");

    ls->num_lettori++;

    pthread_mutex_unlock(&ls->mutex);
}


void fine_lettura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_lock(&ls->mutex);

    ls->num_lettori--;
    
    if(ls->num_lettori==0){

        printf("[LETTURA]: Sono l'ultimo lettore, risveglio uno scrittore\n");
        pthread_cond_signal(&ls->VAR_SCRITT);
    }

    pthread_mutex_unlock(&ls->mutex);
}


void inizio_scrittura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */

    pthread_mutex_lock(&ls->mutex);

    while(ls->num_scritt>0 || ls->num_lettori>0){

        printf("[SCRITTURA]: Non posso scrivere perche' ci sono gia0 altri all'interno del monitor\n");
        ls->num_scrittori_wait++;
        pthread_cond_wait(&ls->VAR_SCRITT, &ls->mutex);
        ls->num_scrittori_wait--;
    }

    printf("[SCRITTURA]: Sono uscito dalla wait\n");
    
    ls->num_scritt++;


    pthread_mutex_unlock(&ls->mutex);
}


void fine_scrittura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_lock(&ls->mutex);

    ls->num_scritt--;

    if(ls->num_scrittori_wait>0){
        pthread_cond_signal(&ls->VAR_SCRITT);        
    }else{
        printf("[SCRITTURA]: Non ci sono piu' scrittori da svegliare, sveglio i lettori\n");
        pthread_cond_broadcast(&ls->VAR_LETT);
    }

    pthread_mutex_unlock(&ls->mutex);
}


void *Scrittore(void *arg) {
    /*
        TODO: recupero parametri thread e inizio scrittura
    */

    ParametriScrittore *ps= (ParametriScrittore*) arg;

    MonitorLettScritt *ls= ps->ls;

   
    inizio_scrittura(ls);

    ls->totale_regali += ps->regali_richiesti;

    
    printf("[SCRITTORE]: Il totale dei regali richiesti finora è: %d\n", ls->totale_regali);

    /*
        TODO: completare
    */
    fine_scrittura(ls);


    pthread_exit(NULL);
}

void *Lettore(void *arg) {
    /*
        TODO: recupero parametri thread
    */

    MonitorLettScritt *ls= (MonitorLettScritt*) arg;

    sleep(3);

    inizio_lettura(ls);

    printf("[Lettore] Il totale di regali da consegnare è: %d\n", ls->totale_regali);

    fine_lettura(ls);

    /*
        TODO: invio messaggio di risposta
    */
    MessaggioRisposta risp;
    risp.val= 0;
    risp.tipo_risp= RISPOSTA;

    int result= msgsnd(coda_risposte, (void *)&risp, sizeof(MessaggioRisposta)-sizeof(long), 0);

    if(result<0){perror("[LETTORE]: Errore nell'invio del messaggio di ricezione\n"); exit(1);}


    printf("[LETTORE]: Ho inviato conferma di ricezione al bambino\n");

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    /*
        TODO: recupero code e inizializzazione strutture
    */
    key_t chiave_coda_richieste= ftok(".", 'f');
    key_t chiave_coda_risposte= ftok(".", 'g');

    coda_richieste= msgget(chiave_coda_richieste, 0);
    coda_risposte= msgget(chiave_coda_risposte, 0);

    MonitorLettScritt *ls= (MonitorLettScritt*) malloc(sizeof(MonitorLettScritt));

    inizializza_struttura(ls);

    ParametriScrittore parametri[NUM_BAMBINI*NUM_LETTERE];

    pthread_t /* TODO */id_threads_lettori[NUM_BAMBINI*NUM_LETTERE];
    pthread_t /* TODO */id_threads_scrittori[NUM_BAMBINI*NUM_LETTERE];

    for (int i = 0; i < NUM_BAMBINI*NUM_LETTERE; i++){ 
        /* TODO */
        printf("[SERVER]: Creo il processo Lettore n. %d\n", i+1);
        pthread_create(&id_threads_lettori[i], NULL, Lettore, (void *)ls);
    }

    MessaggioRichiesta msg;
    for (int i = 0; i < NUM_BAMBINI*NUM_LETTERE; i++) {
        
        int rsult= msgrcv(coda_richieste,&msg,sizeof(MessaggioRichiesta)-sizeof(long), RICHIESTA,0);
        if(rsult<0){perror("[SERVER]: Erorre nella ricezione del messaggio di richiesta\n"); exit(1);}
        /*
            TODO: ricezione messaggio, inizializzazione della struttura parametri da passare al thread e creazione scrittore
        */
        printf("[SERVER]: Ho ricevuto una letterina! Sono richiesti %d regali\n", msg.numero_regali);

        parametri[i].regali_richiesti= msg.numero_regali;
        parametri[i].ls= ls;

        printf("[SERVER]: Creo il Processo Scrittore n. %d \n", i+1);
        pthread_create(&id_threads_scrittori[i], NULL, Scrittore, (void*)&parametri[i]);



    }


    /*
        TODO: attendo terminazione thread e dealloco strutture
    */
    for (int i = 0; i < NUM_BAMBINI*NUM_LETTERE; i++){
        pthread_join(id_threads_lettori[i], NULL);
        printf("[SERVER]: TERMINAZIONE del Thread Lettore n. %d\n", i+1);
    }

    for (int i = 0; i < NUM_BAMBINI*NUM_LETTERE; i++){
        pthread_join(id_threads_scrittori[i], NULL);
        printf("[SERVER]: TERMINAZIONE del Thread Scrittore n. %d\n", i+1);
    }


    distruggi_struttura(ls);
    free(ls);

    return 0;

}