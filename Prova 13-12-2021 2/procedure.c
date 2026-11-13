#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#include "header.h"

void inizializza(MonitorCoda *p) {

    /* TBD: Aggiungere codice per l'inizializzazione del monitor e delle relative
     * variabili per la sincronizzazione
     */
     init_monitor(&p->m, 2);

     p->num_lib=DIMENSIONE;
     p->num_occ= 0;
     p->produttori_in_attesa=0;
     

    for(int j=0; j<DIMENSIONE; j++){
     p->vettore[j]= 0;
     }


     for(int j=0; j<DIMENSIONE; j++){
     p->stato[j]= LIBERO;
     }
    

}

void rimuovi(MonitorCoda * p) {
    /* TBD: Aggiungere codice per la rimozione del monitor */
    remove_monitor(&p->m);
}


int produzione(MonitorCoda *p, int valore) {

    /* TBD: Aggiungere codice per la produzione in accordo allo schema
     * con vettore di stato, ricordando che il valore di ritorno della funzione
     * indica se il monitor è sovraccarico o meno.
     * Nel caso in cui ci siano almeno 4 produttori in attesa, bisogna forzare
     * l'uscita del processo dal monitor e ritornare un valore pari ad 1.
     * Nel caso in cui la produzione avviene normalmente ritornare un valore pari a 0.
     */

     enter_monitor(&p->m);


     if(p->num_lib==0){
        printf("[PRODUZIONE]: Buffer pieno, attendo...\n");
        if(p->produttori_in_attesa>=4){/*printf("[Sovraccarico, esco]\n");*/ leave_monitor(&p->m); return 1;}
        p->produttori_in_attesa++;
        wait_condition(&p->m, VAR_PROD);
        p->produttori_in_attesa--;
     }

     int i=0;

     while(i<DIMENSIONE && p->stato[i]!= LIBERO){
     i++;
     }

     p->num_lib--;
     p->stato[i]= IN_USO;

     leave_monitor(&p->m);



     p->vettore[i]= valore;



     enter_monitor(&p->m);

     p->stato[i]= OCCUPATO;
     p->num_occ++;

     signal_condition(&p->m, VAR_CONS);

     leave_monitor(&p->m);

     return 0;

}


/* NOTA: il valore di ritorno di consumazione() è il valore prelevato dalla coda */

int consumazione(MonitorCoda *p) {


    /* TBD: Aggiungere codice per la consumazione in accordo allo schema
     * con vettore di stato, ricordando che il valore di ritorno della funzione
     * indica il valore prelevato dal buffer condiviso.
     */

     enter_monitor(&p->m);

     if(p->num_occ==0){
        printf("[CONSUMAZIONE]: Buffer vuoto, attendo\n");
        wait_condition(&p->m, VAR_CONS);
     }

     int i= 0;

     while(i<DIMENSIONE && p->stato[i]!=OCCUPATO){
        i++;
     }

     p->num_occ--;
     p->stato[i]= IN_USO;

     leave_monitor(&p->m);


     int valore= p->vettore[i];


     enter_monitor(&p->m);

     p->stato[i]= LIBERO;
     p->num_lib++;

    signal_condition(&p->m, VAR_PROD);

    leave_monitor(&p->m);

    return valore;
}

void produttore(MonitorCoda *m) {
    
    int valore;
    int ret;
    int i;
    
    for(i=0; i<5; i++) {
        
        while(1) {
            
            srand(time(NULL)*getpid());
            
            valore = rand() % 10;
            
            ret = produzione(m, valore);
            
            if(ret==0) { break; }
            
            printf("MONITOR SOVRACCARICO, ATTENDO 3 secondi...\n");
            
            sleep(3);
        }
        
        printf("VALORE PRODOTTO: %d\n", valore);
        
        sleep(1);
    }
    
    
    exit(0);
}

void consumatore(MonitorCoda *m) {
    
    int i;
    int valore;
    
    for(i=0; i<15; i++) {
        
        valore = consumazione(m);
        
        printf("VALORE CONSUMATO: %d\n", valore);
        
        sleep(2);
    }
    
    
    exit(0);
}

