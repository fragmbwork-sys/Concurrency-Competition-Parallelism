#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

int main(){
    
    //Creazione code richieste e risposte
    int ds_queue_request, ds_queue_response;

    //create queues
    int key_queue_request = ftok("./", 'f');// TODO aggiungere codice per la generazione della chiave
    int key_queue_response = ftok("./", 'g');// TODO aggiungere codice per la generazione della chiave
    
    // TODO aggiungere codice di inizializzazione delle code
    ds_queue_request = msgget(key_queue_request, 0);//...
    ds_queue_response = msgget(key_queue_response, 0);//...
    
    printf("[viewer] Coda ds_queue_request ID: %d\n", ds_queue_request);
    printf("[viwwer] Coda ds_queue_response ID: %d\n", ds_queue_response);
    
    
    Msg_Req m_req;
    Msg_Resp m_res;

   
    int i;

    srand(time(NULL)*getpid());
    
    for (i=0; i<4; i++){
        
        // TODO costruzione del messaggio di richiesta
        m_req.tipo_richiesta= READ;
        m_req.id_prodotto= rand()%3;
        m_req.value= 0;
        m_req.mittente= getpid();

        // TODO aggiungere codice per inviare la richiesta
        int result= msgsnd(ds_queue_request, (void *)&m_req, sizeof(Msg_Req)-sizeof(long), 0);

        if(result<0){perror("[VIEWER]: Errore nell'invio di un messaggio richiesta\n"); exit(1);}
        
        // TODO aggiungere codice per ricevere la risposta
        int result2= msgrcv(ds_queue_response, (void *)&m_res, sizeof(Msg_Resp)-sizeof(long), getpid(), 0);
        
        if(result2<0){perror("[VIEWER]: Errore nella ricezione di un messaggio di risposta\n"); exit(1);}


        printf("[viewer %d] Sono disponibili %d unità del prodotto %d\n", getpid(), m_res.scorta,m_res.id_prodotto);
    }
    
    return 0;
}


