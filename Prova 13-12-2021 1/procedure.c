#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "header.h"

int coda_RTS;
int coda_OTS;


void initServiceQueues(){

    /* TBD: inizializzazione code per implementare la send sincrona */
    coda_RTS = msgget(IPC_PRIVATE, IPC_CREAT|0664);/* TBD */

    if(coda_RTS<0){perror("[MAIN]: Errore nella creazione coda RTS\n"); exit(1);}
    printf("Coda di servizio coda_RTS create con desc: %d\n", coda_RTS);
    
    coda_OTS = msgget(IPC_PRIVATE, IPC_CREAT|0664);/* TBD */
    if(coda_OTS<0){perror("[MAIN]: Errore nella creazione coda OTS\n"); exit(1);}
    printf("Coda di servizio coda_OTS create con desc: %d\n", coda_OTS);
}

void removeServiceQueues(){
    /* TBD: rimozione code per implementare la send sincrona */
    msgctl(coda_RTS, IPC_RMID, NULL);
    msgctl(coda_OTS, IPC_RMID, NULL);
}


void controllore(int ds_queue_control_proxy){
    
    int i, ret;
    char citta_partenza[][20] = { "Napoli", "Roma", "Milano", "Torino", "Firenze"};
    char citta_destinazione[][20] = { "New York", "Madrid", "Londra", "Berlino", "Parigi"};
    
    Volo v_partenza;
    Volo v_arrivo;
    
    for (i=0; i<10; i++){
        
        /* TBD: Implementare lo scambio di messaggi necessario per
         * avere una send sincrona */
        Control c;
        c.tipo= REQUEST_TO_SEND;
        c.valore= 0;

        int result= msgsnd(coda_RTS, (void *)&c, sizeof(Control)-sizeof(long), 0);

        if(result<0){perror("[CONTROLLORE]: Errore nella send della RTS\n"); exit(1);} 

        Control c2;

        int result2= msgrcv(coda_OTS, (void*)&c2, sizeof(Control)-sizeof(long), OK_TO_SEND, 0);

        if(result2<0){perror("[CONTROLLORE]: Errore nella receive della OTS\n"); exit(1);}
        


        if (i%2 == 0){
            
            /* TBD: Implementare invio messaggi per voli in ARRIVO */
            v_arrivo.tipo= MSG_TYPE_CONTROL_PROXY;
            strcpy(v_arrivo.citta_arrivo, citta_destinazione[rand()%5]);
            strcpy(v_arrivo.citta_partenza, citta_partenza[rand()%5]);
            strcpy(v_arrivo.direzione, "arrivo");
            v_arrivo.ID= (rand()%1000);
            
            int rsult= msgsnd(ds_queue_control_proxy, (void *)&v_arrivo, sizeof(Volo)-sizeof(long), 0);

            if(rsult<0){perror("[CONTROLLORE_ARR]: Errore nell'invio di un messaggio verso il Proxy\n"); exit(1);}
            
        }else {

            /* TBD: Implementare invio messaggi per voli in PARTENZA */
            v_partenza.tipo= MSG_TYPE_CONTROL_PROXY;
            strcpy(v_partenza.citta_arrivo, citta_destinazione[rand()%5]);
            strcpy(v_partenza.citta_partenza, citta_partenza[rand()%5]);
            strcpy(v_partenza.direzione, "partenza");
            v_partenza.ID= (rand()%1000);

            
            int rsult= msgsnd(ds_queue_control_proxy, (void *)&v_partenza, sizeof(Volo)-sizeof(long), 0);

            if(rsult<0){perror("[CONTROLLORE_PAR]: Errore nell'invio di un messaggio verso il Proxy\n"); exit(1);}
        }
        
    }
    exit(0);
    
}
void proxy(int ds_queue_control_proxy, int ds_queue_proxy_gestori){
    
    int i, ret;
    Volo v;
    for (i=0; i<10; i++){
        
        /* TBD: Implementare lo scambio di messaggi necessario per
         * avere una send sincrona */
        Control c;

        int result= msgrcv(coda_RTS, (void *)&c, sizeof(Control)-sizeof(long), REQUEST_TO_SEND, 0);

        if(result<0){perror("[Proxy]: Errore nella ricezione della RTS\n"); exit(1);}

        Control c2;

        c2.tipo= OK_TO_SEND;
        c2.valore=0;

        int result2= msgsnd(coda_OTS, (void *)&c2, sizeof(Control)-sizeof(long), 0);

        if(result2<0){perror("[Proxy]: Errore nell'invio della OTS\n"); exit(1);}
        
        
        /* TBD: Aggiungere la ricezione del volo vero e proprio */
        int result3= msgrcv(ds_queue_control_proxy, (void *)&v, sizeof(Volo)-sizeof(long), MSG_TYPE_CONTROL_PROXY, 0);

        if(result3<0){perror("[Proxy]: Errore nella ricezione del messaggio dal Controllore\n"); exit(1);}
        
        /* TBD: Aggiungere il codice per inviare opportunamente il volo
         * ricevuto al Gestore Arrivi o al Gestore Partenze
         */
        
        if(strcmp(v.direzione, "partenza")==0){
            //printf("Il messaggio e' di partenza.\n");
            /*Volo risposta;
            strcpy(risposta.citta_partenza, v.citta_partenza);
            strcpy(risposta.citta_arrivo, v.citta_arrivo);
            strcpy(risposta.direzione, v.direzione);
            risposta.ID= v.ID;
            risposta.tipo= MSG_TYPE_PARTENZE;*/
            v.tipo= MSG_TYPE_PARTENZE;

            int result= msgsnd(ds_queue_proxy_gestori,(void *)&v, sizeof(Volo)-sizeof(long), 0);
            if(result<0){perror("[Proxy]: Errore nell'invio del messaggio ai gestori di partenza\n"); exit(1);}

        }else if(strcmp(v.direzione, "arrivo")==0){
            //printf("Il messaggio e' di arrivo.\n");
            /*Volo risposta;
            strcpy(risposta.citta_partenza, v.citta_partenza);
            strcpy(risposta.citta_arrivo, v.citta_arrivo);
            strcpy(risposta.direzione, v.direzione);
            risposta.ID= v.ID;
            risposta.tipo= MSG_TYPE_ARRIVI;*/
            v.tipo= MSG_TYPE_ARRIVI;

            int result= msgsnd(ds_queue_proxy_gestori,(void *)&v, sizeof(Volo)-sizeof(long), 0);
            if(result<0){perror("[Proxy]: Errore nell'invio del messaggio ai gestori di arrivo\n"); exit(1);}
        }else{
            printf("[PROXY]: Errore non ho capito il tipo del messaggio\n");
        }
        
    }
    exit(0);
}

void stampa_volo_info(Volo *v){
    
    printf("STAMPO INFO VOLO\n");
    printf("..........................ID: %d\n", v->ID);
    printf("..........................direzione: %s\n", v->direzione);
    printf("..........................citta_partenza: %s\n", v->citta_partenza);
    printf("..........................citta_arrivo: %s\n", v->citta_arrivo);
    
}

void gestore_arrivi(int ds_queue_proxy_gestori){
    
    int ret, i;
    Volo v;
    
    for (i=0; i<5; i++){
        
        /* TBD: Ricezione voli in ARRIVO */
        int result= msgrcv(ds_queue_proxy_gestori, (void *)&v, sizeof(Volo)-sizeof(long), MSG_TYPE_ARRIVI, 0);

        if(result<0){perror("[GESTORE_ARRIVI]: Errore nella ricezione di un messaggio\n"); exit(1);}
        
        printf("\n\n****** ARRIVI ******\n\n");
        stampa_volo_info(&v);
    }
    exit(0);
}

void gestore_partenze(int ds_queue_proxy_gestori){
    
    int ret, i;
    Volo v;
    for (i=0; i<5; i++){
        
        /* TBD: Ricezione voli in PARTENZA */
        int result= msgrcv(ds_queue_proxy_gestori, (void *)&v, sizeof(Volo)-sizeof(long), MSG_TYPE_PARTENZE, 0);

        if(result<0){perror("[GESTORE_PARTENZE]: Errore nella ricezione di un messaggio\n"); exit(1);}
        
        printf("\n\n****** PARTENZE ******\n\n");
        stampa_volo_info(&v);
    }
    exit(0);
}

