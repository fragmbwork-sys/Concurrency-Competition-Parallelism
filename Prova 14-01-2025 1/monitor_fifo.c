#include "monitor_fifo.h"

#include <stdio.h>
#include <unistd.h>

void inizializza(MonitorProdConsFIFO * p) {

    /* TBD: Inizializzare la struttura dati */
    p->num_posti_liberi= DIM;
    p->num_posti_occupati= 0;
    p->ultimo_produttore_arrivato= 0;
    p->ultimo_produttore_avviato= 0;

    init_monitor(&(p->m), 2);

    for(int i=0; i<DIM; i++){

        p->buffer[i]= 0;
        p->vett_stato[i]= LIBERO;
    }
    
}

void produci(MonitorProdConsFIFO * p, int valore) {

    /* TBD: Completare con la sincronizzazione */

    enter_monitor(&(p->m));


    int arrivo = p->ultimo_produttore_arrivato;

    p->ultimo_produttore_arrivato++;

    printf("[PRODUTTORE %d] Il mio turno di arrivo è %d\n", getpid(), arrivo);


    // ... gestire le condizioni di attesa ...

    while (p->num_posti_liberi== 0 || arrivo!= p->ultimo_produttore_avviato){

        printf("[PRODUCI]: Sono entrato nel monitor ma non posso ancora produrre perche' non c'e' spazio o perche' le variabili non coincidono\n");
        wait_condition(&(p->m), SPAZIO_LIB);
    }

    //p->ultimo_produttore_avviato++; //SE METTO QUI FUNZIONA

    int i= 0;
    while(i<DIM && p->vett_stato[i]!=LIBERO){

        i++;
    }

    p->vett_stato[i]= IN_USO;
    p->num_posti_liberi--;


    printf("[PRODUTTORE %d] Il mio turno di produzione è %d\n", getpid(), p->ultimo_produttore_avviato);

    p->ultimo_produttore_avviato++; //ANCHE QUA FUNZIONA MA IL TURNO DI PRODUZIONE VA DA 0 A 19 (CORRETTO) E NON DA 1 A 20

    leave_monitor(&(p->m));



    p->buffer[/* TBD */i] = valore;


    enter_monitor(&(p->m));

    p->vett_stato[i]= OCCUPATO;
    p->num_posti_occupati++;

    signal_condition(&(p->m), MESS_DISP);

    leave_monitor(&(p->m));

    sleep(1);

}

int consuma(MonitorProdConsFIFO * p) {

    /* TBD: Completare con la sincronizzazione */

    enter_monitor(&(p->m));

    while(p->num_posti_occupati==0){

        printf("[CONSUMA]: Devo rimanere in attesa perche' non ci sono ancora elementi da consumare\n");
        wait_condition(&(p->m), MESS_DISP);
    }

    int j=0;

    while (j<DIM && p->vett_stato[j]!=OCCUPATO){
        j++;
    }

    p->vett_stato[j]= IN_USO;
    p->num_posti_occupati--;

    leave_monitor(&(p->m));


    printf("[CONSUMATORE %d] Consumazione in corso\n", getpid());

    int valore = p->buffer[/* TBD */j];

    enter_monitor(&(p->m));

    p->vett_stato[j]= LIBERO;
    p->num_posti_liberi++;


    sleep(1);


    /* Nota: Occorre riattivare -tutti- i produttori in attesa.
             Il numero di produttori in attesa è dato dalla seguente differenza.
     */

    int produttori_in_attesa = p->ultimo_produttore_arrivato - p->ultimo_produttore_avviato;

    if(produttori_in_attesa!=0)
    signal_all(&(p->m), SPAZIO_LIB);

    leave_monitor(&(p->m));


    return valore;
}

void distruggi(MonitorProdConsFIFO * p) {

    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&(p->m));

}
