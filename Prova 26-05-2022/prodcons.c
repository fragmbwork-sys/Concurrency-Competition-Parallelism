#include <stdio.h>

#include "prodcons.h"


void init_monitor_circolare(MonitorVettoreCircolare * p) {

    /* TBD: Inizializzare il monitor */
    p->testa=0;
    p->coda=0;
    p->nelem=0;
    
    init_monitor(&p->c, 2);

    for(int j=0; j<DIM; j++){
        p->buffer[j]= 0;
    }

}

void produzione_circolare(MonitorVettoreCircolare * p, int val) {

    /* TBD: Completare la produzione nel buffer circolare con sincronizzazione */

    enter_monitor(&p->c);

    if(p->nelem==DIM){
        printf("[PRODUTTORE_CIRCOLARE]: Vettore pieno, attendo\n");
        wait_condition(&p->c, VAR_PROD);
    }

    printf("[PRODUTTORE_CIRCOLARE]: Sono uscito dall'attesa\n");

    
    p->nelem++;
    p->buffer[/* TBD */p->coda] = val;
    p->coda= (p->coda+1)%DIM;
    

    
    signal_condition(&p->c, VAR_CONS);

    leave_monitor(&p->c);

}

int consumazione_circolare(MonitorVettoreCircolare * p) {

    /* TBD: Completare la consumazione dal buffer circolare con sincronizzazione */

    int val;

    enter_monitor(&p->c);

    if(p->nelem==0){
        printf("[CONSUMATORE_CIRCOLARE]: Vettore vuoto, attendo\n");
        wait_condition(&p->c, VAR_CONS);
    }

    printf("[CONSUMATORE_CIRCOLARE]: Sono uscito dall'attesa\n");

    p->nelem--;
    val = p->buffer[/* TBD */p->testa];
    p->testa= (p->testa+1)%DIM;
    

    signal_condition(&p->c, VAR_PROD);

    leave_monitor(&p->c);

    return val;
}

void remove_monitor_circolare(MonitorVettoreCircolare * p) {

    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&p->c);
}



void init_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Inizializzare il monitor */
    b->nelem= 0;
    b->buffer= 0;

    init_monitor(&b->b, 2);
    
}

void produzione_buffer_singolo(MonitorBufferSingolo * b, int val) {

    /* TBD: Completare la produzione sul buffer singolo con sincronizzazione */

    enter_monitor(&b->b);

    if(b->nelem>0){
        printf("[PRODUZIONE_SINGOLO]: Buffer pieno, attendo\n");
        wait_condition(&b->b, VAR_PROD);
    }

    printf("[PRODUZIONE_SINGOLO]: Sono uscito dall'attesa\n");

    b->nelem++;
    b->buffer = val;
    

    signal_condition(&b->b, VAR_CONS);

    leave_monitor(&b->b);

}

int consumazione_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Completare la consumazione dal buffer singolo con sincronizzazione */

    enter_monitor(&b->b);

    if(b->nelem==0){
        printf("[CONSUMAZIONE_SINGOLO]: Buffer vuoto, attendo\n");
        wait_condition(&b->b, VAR_CONS);
    }

    printf("[CONSUMAZIONE_SINGOLO]: Sono uscito dall'attesa\n");

    int val;

    b->nelem--;
    val = b->buffer;
    

    signal_condition(&b->b, VAR_PROD);

    leave_monitor(&b->b);

    return val;
}

void remove_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&b->b);
}
