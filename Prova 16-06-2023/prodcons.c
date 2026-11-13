#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"


void init_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Inizializzare il monitor */
    b->buffer_pieno=0;
    b->buffer_vuoto=1;
    b->num_produzioni=0;
    b->buffer=0;
    init_monitor(&(b->m), 2);
    
}

void produzione_buffer_singolo(MonitorBufferSingolo * b, int val) {

    /* TBD: Completare la produzione sul buffer singolo con sincronizzazione */
    enter_monitor(&(b->m));

    if(b->buffer_pieno==1){
        printf("[PRODUZIONE_BUFFER]: Non posso ancora produrre pe rvia di buffer pieno\n");
        wait_condition(&(b->m), VARCOND_PRODUTTORI);
    }

    printf("[PRODUZIONE_BUFFER]: Sono uscito dall'attesa\n");

    b->buffer= val;
    b->buffer_pieno= 1;
    b->buffer_vuoto= 0;

    signal_condition(&(b->m), VARCOND_CONSUMATORI);

    leave_monitor(&(b->m));

}

int consumazione_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Completare la consumazione dal buffer singolo con sincronizzazione */

    enter_monitor(&(b->m));
    
    if(b->buffer_vuoto==1){
        printf("[CONSUMAZIONE_BUFFER]: Non c'è niente da consumare, attendo\n");
        wait_condition(&(b->m), VARCOND_CONSUMATORI);
    }

    printf("[CONSUMAZIONE_BUFFER]: Sono uscito dall'attesa\n");

    int val = b->buffer;
    b->buffer_vuoto= 1;
    b->buffer_pieno= 0;

    signal_condition(&(b->m), VARCOND_PRODUTTORI);

    leave_monitor(&(b->m));

   
    return val;
}

void remove_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Disattivare l'oggetto monitor */

    remove_monitor(&(b->m));
}
