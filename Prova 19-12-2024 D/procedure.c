#include "header.h"
#include "monitor_signal_continue.h"

void inizializza_monitor(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
   ls->totale_regali= 0;
   ls->num_lettori= 0;
   ls->num_scrittori= 0;
   init_monitor(&(ls->m), 2);
}

void distruggi_monitor(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
   remove_monitor(&(ls->m));
}

void inizio_lettura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
   enter_monitor(&(ls->m));

   

   while(ls->num_scrittori>0){
    printf("[INIZIO_LETTURA]: C'e' un processo scrittore attendo...\n");
    wait_condition(&(ls->m), VAR_LETT);
   }

   ls->num_lettori++;
    
   // signal_condition(&(ls->m), VAR_LETT); aggiungo questa istruzione nel caso di Monitor Signal and Continue
   
   
   leave_monitor(&(ls->m));


}

void fine_lettura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */

    enter_monitor(&(ls->m));

    ls->num_lettori--;

    if(ls->num_lettori==0){
        printf("[FINE_LETTURA]: Risveglio un processo Scrittore...\n");
        signal_condition(&(ls->m), VAR_SCRITT);
    }

    leave_monitor(&(ls->m));


}

void inizio_scrittura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */

    enter_monitor(&(ls->m));

    while(ls->num_lettori>0 || ls->num_scrittori>0){
        printf("[INIZIO_SCRITTURA]: Ci sono gia' lettori/scrittori, attendo...\n");
        wait_condition(&(ls->m), VAR_SCRITT);
    }

    ls->num_scrittori++;

    leave_monitor(&(ls->m));
}

void fine_scrittura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */

    enter_monitor(&(ls->m));

    ls->num_scrittori--;

    if(queue_condition(&(ls->m), VAR_SCRITT)>0){
        printf("[FINE_SCRITTURA]: Ci sono ancora processi scrittori in attesa, ne sveglio uno...\n");
        signal_condition(&(ls->m), VAR_SCRITT);
    }else if(queue_condition(&(ls->m), VAR_LETT)>0){
        printf("[FINE_SCRITTURA]: Non ci sono altri processi scrittori in attesa, sveglio un lettore...\n");
        signal_all(&(ls->m), VAR_LETT);
    }

    leave_monitor(&(ls->m));
}

void Lettore(int codaresp, MonitorLettScritt *ls) {

	/*
        TODO: completare
    */

    inizio_lettura(ls);

    printf("[Lettore] Il totale regali richiesti finora è: %d\n", ls->totale_regali);

    fine_lettura(ls);


    /*
        TODO: invio messaggio conferma di ricezione 
    */
   MessaggioRisposta risp;
   risp.tipo_risp= RISPOSTA;

   int result= msgsnd(codaresp, (void*) &risp, sizeof(MessaggioRisposta)- sizeof(long), 0);

   if(result<0){perror("[WARNING_LETTORE]: Errore nell'invio di un messaggio di risposta\n");}

    printf("[Lettore] Ho inviato la conferma di ricezione\n");
}

void Scrittore(int codareq, MonitorLettScritt *ls) {
    /*
        TODO: attesa messaggio
    */
   MessaggioRichiesta mess;
   
   int result= msgrcv(codareq, (void*) &mess, sizeof(MessaggioRichiesta)-sizeof(long), RICHIESTA, 0);

   if(result<0){perror("[WARNING_SCRITTORE]: Errore nella ricezione di un messaggio di richiesta\n");}


	/*
        TODO: completare scrittura
    */

    inizio_scrittura(ls);

    printf("[SCRITTORE]: Inizio la scrittura... \n");
    ls->totale_regali= ls->totale_regali + mess.numero_regali;
    printf("[SCRITTORE]: Finisco la scrittura... \n");

    printf("[Scrittore %d] Adesso il totale dei regali richiesti è: %d\n",getpid(), ls->totale_regali);

    fine_scrittura(ls);

}
