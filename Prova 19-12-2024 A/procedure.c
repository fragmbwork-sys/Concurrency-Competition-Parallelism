#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#include "header.h"
#include "semafori.h"

void inizializza_vettore(MonitorProdCons *pc) {
    /*
        TODO: completare
    */
    pc->nelem_liberi=DIM;
    pc->nelem_occupati= 0;
    pc->nelem_tipo1= 0;
    pc->nelem_tipo2= 0;
    
    for(int i=0; i<DIM; i++){
        pc->stato[i]= LIBERO;
    }

    for(int i=0; i<DIM; i++){
        pc->vettore[i]= 0;
    }

    init_monitor(&pc->m, 3);

}

void rimuovi_vettore(MonitorProdCons *pc) {
    /*
        TODO: completare
    */
    remove_monitor(&pc->m);

}

void inizializza_buffer(BufferBabbo * bb) {

	/*
        TODO: inizializzazione array di semafori necessari e inizializzazione struttura BufferBabbo
    */
    int id_sem= semget(IPC_PRIVATE, 3, IPC_CREAT|0664);

    if(id_sem<0){
        printf("[MAIN]: Errore nella creazione della coda dei semafori\n");
        exit(1);
    }

    bb->buffer= 0;
    bb->semid= id_sem;

    semctl(bb->semid, MUTEX, SETVAL, 1);
    semctl(bb->semid, SPAZIO_DISP, SETVAL, 1);
    semctl(bb->semid, MESS_DISP, SETVAL, 0);
}


void rimuovi_buffer(BufferBabbo * bb) {
    /*
        TODO: completare
    */
    semctl(bb->semid, 0, IPC_RMID);
    
}

void Produttore(MonitorProdCons *pc) {

    for (int i=0; i<TOT_GIOCATTOLI; i++) {
        if (i%2==0) {
            /*
        		TODO: completare
    		*/
            int tipo1= 1;
            produci(pc, tipo1);
        } else {
            /*
        		TODO: completare
    		*/
            int tipo2= 2;
            produci(pc, tipo2);
        }

		sleep(2);
    }

}

void produci(MonitorProdCons *pc, int tipo) {

    /*
        TODO: produzione con monitor e vettore di stato
    */
    enter_monitor(&pc->m);

	/*
        TODO: completare
    */
    if(pc->nelem_liberi==0){
        printf("[PRODUCI]: Non c'e' spazio e quindi non posso produrre\n");
        wait_condition(&pc->m, COND_PROD);
    }

    int i=0;
    while(i<DIM && pc->stato[i]!=LIBERO){
    i++;
    }

    pc->stato[i]= IN_USO;
    pc->nelem_liberi--;

    leave_monitor(&pc->m);

    sleep(1);



	printf("Produzione - posizione %d, giocattolo %d\n", i, tipo);

    pc->vettore[i]= tipo;

    
    
    enter_monitor(&pc->m);


    pc->nelem_occupati++;

    if (tipo == 1) {
        /*
        	TODO: completare
    	*/
        pc->stato[i]= OCCUPATO_GIOCO1;
        pc->nelem_tipo1++;
        signal_condition(&pc->m, COND_CONS1);
        
    } else if (tipo == 2) {
        /*
        	TODO: completare
    	*/
        pc->stato[i]= OCCUPATO_GIOCO2;
        pc->nelem_tipo2++;
        signal_condition(&pc->m, COND_CONS2);

    }

    leave_monitor(&pc->m);

}

void Consumatore1(MonitorProdCons *pc, BufferBabbo *bb) {

    int gioco;

	/*
        TODO: consumo con monitor e vettore di stato
    */for(int j=0; j<10; j++){
    
    enter_monitor(&pc->m);

    if(pc->nelem_tipo1==0){
        printf("[CONSUMATORE1]: Non ci sono elementi da consumare\n");
        wait_condition(&pc->m, COND_CONS1);
    }

    int i=0;

    while(i<DIM && pc->stato[i]!=OCCUPATO_GIOCO1){
        i++;
    }

    pc->stato[i]= IN_USO;
    pc->nelem_occupati--;
    pc->nelem_tipo1--;

    leave_monitor(&pc->m);


    gioco= pc->vettore[i];
	printf("Consumo1 - posizione %d, gioco %d\n", i, gioco);

	/*
        TODO: consumo con monitor e vettore di stato
    */

    enter_monitor(&pc->m);


    pc->nelem_liberi++;
    pc->stato[i]= LIBERO;

    signal_condition(&pc->m, COND_PROD);

    leave_monitor(&pc->m);


    /*
        TODO: produzione di un aggiornamento del buffer in mutua esclusione
    */
    
    
    Wait_Sem(bb->semid, SPAZIO_DISP);


    Wait_Sem(bb->semid, MUTEX);

	printf("Elfo Assemblatore 1, incremento il buffer\n");

    bb->buffer= bb->buffer+1;

    Signal_Sem(bb->semid, MUTEX);


    Signal_Sem(bb->semid, MESS_DISP);

    
    }
}

void Consumatore2(MonitorProdCons *pc, BufferBabbo *bb) {
    int gioco;

	/*
        TODO: consumo con monitor e vettore di stato
    */for(int j=0; j<10; j++){

    enter_monitor(&pc->m);

    if(pc->nelem_tipo2==0){
        printf("[CONSUMATORE2]: Non ci sono elementi da consumare\n");
        wait_condition(&pc->m, COND_CONS2);
    }

    
    int i=0;

    while(i<DIM && pc->stato[i]!=OCCUPATO_GIOCO2){
        i++;
    }

    pc->stato[i]= IN_USO;
    pc->nelem_occupati--;
    pc->nelem_tipo2--;

    leave_monitor(&pc->m);


    gioco= pc->vettore[i];
	printf("Consumo2 - posizione %d, gioco %d\n", i, gioco);

	/*
        TODO: consumo con monitor e vettore di stato
    */

    enter_monitor(&pc->m);


    pc->nelem_liberi++;
    pc->stato[i]= LIBERO;

    signal_condition(&pc->m, COND_PROD);

    leave_monitor(&pc->m);


    /*
        TODO: produzione di un aggiornamento del buffer in mutua esclusione
    */

    
    Wait_Sem(bb->semid, SPAZIO_DISP);

    
    Wait_Sem(bb->semid, MUTEX);
    
	printf("Elfo Assemblatore 2, incremento il buffer\n");

    bb->buffer= bb->buffer+1;

    Signal_Sem(bb->semid, MUTEX);


    Signal_Sem(bb->semid, MESS_DISP);

    
    }
}

void BabboNatale(BufferBabbo *bb) {

	/*
        TODO: consumo di un aggiornamento del buffer in mutua esclusione
    */ 

    for(int j=0; j<TOT_GIOCATTOLI; j++){

    Wait_Sem(bb->semid, MESS_DISP);

    
    Wait_Sem(bb->semid, MUTEX);    

    printf("Sono Babbo Natale, ho consegnato un totale di %d regali\n",bb->buffer);
    
    Signal_Sem(bb->semid, MUTEX);


    Signal_Sem(bb->semid, SPAZIO_DISP);

    

    
    }


}