#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#include "header.h"

typedef struct {

    int *vettore;

    /* TBD: Aggiungere altre variabili per la sincronizzazione */

    pthread_mutex_t mutex;



} risorsa_database;


typedef struct {

    /* TBD: Utilizzare questa struttura dati per il
            passaggio dei parametri dal thread principale ai
            thread worker.
     */
    //int id_thread;
    int id_coda_richieste;
    int id_coda_risposta;
    risorsa_database *risorsa;

} parametri_worker;


void * worker(void * x);


#define THREAD_WORKER 3


int main() {

    // risorsa gestita dal database (vettore di interi)

    risorsa_database * r = (risorsa_database*) malloc(sizeof(risorsa_database));

    r->vettore = malloc(sizeof(int) * DIM_VETTORE);

    for(int i=0; i<DIM_VETTORE; i++) {
        r->vettore[i] = 0;
    }


    /* TBD: Completare l'inizializzazione delle variabili di sincronizzazione */
    pthread_mutex_init(&r->mutex, NULL);


    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t key_coda1= ftok(".", 'f');
    key_t key_coda2= ftok(".", 'g');
 
    int idmess_req= msgget(key_coda1, 0);
    int idmess_risp= msgget(key_coda2, 0);



    /* TBD: Creare 3 thread worker, e utilizzare la struttura "parametri_worker"
            per passare ai thread il puntatore alla risorsa e gli
            identificativi delle code di messaggi.
     */
    parametri_worker par[THREAD_WORKER];
    pthread_attr_t attr;
    pthread_attr_init (&attr);
    pthread_attr_setdetachstate(&(attr), PTHREAD_CREATE_JOINABLE);

    pthread_t id_thread[THREAD_WORKER];


    for(int i=0; i<THREAD_WORKER; i++) {

        /* TBD */
        //id_thread[i]= i+1;
        par[i].risorsa= r;
        par[i].id_coda_richieste= idmess_req;
        par[i].id_coda_risposta= idmess_risp;
        int err= pthread_create(&id_thread[i], &(attr), worker, (void*)&par[i]);
        
        if(err){
            printf("[WARNING_DATABASE]: Errore nella creazione di un thread worker\n");
            exit(1);
        }
    }

    /* TBD: Attendere la terminazione dei thread worker */
    for(int k=0; k<THREAD_WORKER; k++){
        printf("[DATABASE]: Terminazione del thread n. %d\n", k+1);
        pthread_join(id_thread[k], NULL);
    }



    free(r->vettore);
    free(r);

}



void * worker(void * x) {

    /* TBD: Completare il passaggio dei parametri */
    parametri_worker* parametri= (parametri_worker*) x;
    risorsa_database * r = /* TBD */parametri->risorsa;
    

    
    for(int i=0; i<NUMERO_RICHIESTE; i++) {

        /* TBD: Ricevere un messaggio di richiesta */
        messaggio_richiesta mess;

        int result= msgrcv(parametri->id_coda_richieste, (void*)&mess, sizeof(messaggio_richiesta)-sizeof(long), 0, 0);

        if(result<0){perror("[WARNING_DATABASE]: Errore nella ricezione di un messaggio\n");}

        int tipo_richiesta = /* TBD */mess.tipo;
        int valore = /* TBD */mess.valoreM;
        int posizione = /* TBD */mess.posizione;
        int pid_server = /* TBD */mess.identificativo;

        printf("[DB] Ricevuta richiesta (tipo=%d, pid=%d, posizione=%d, valore=%d)\n", tipo_richiesta, pid_server, posizione, valore);


        int valore_risposta;

        /* TBD: Aggiungere la sincronizzazione per l'accesso alla risorsa condivisa */
        pthread_mutex_lock(&(r->mutex));

        if(mess.tipo== 1/* TBD: Verificare se è una richiesta di LETTURA */) {

            valore_risposta = r->vettore[posizione];
            printf("[DATABASE]: Ottenuta una richiesta di LETTURA. Valore risposta: %d\n", valore_risposta);

        }
        else if(mess.tipo== 2/* TBD: Verificare se è una richiesta di SCRITTURA */) {

            r->vettore[posizione] = valore;
            valore_risposta = 0;
            printf("[DATABASE]: Ottenuta una richiesta di SCRITTURA\n");

        }
        else {
            printf("[DB] Tipo richiesta non valido\n");
        }

        pthread_mutex_unlock(&(r->mutex));



        /* TBD: Inviare un messaggio di risposta, includendo la variabile "valore_risposta" */
        messaggio_risposta risp;
        risp.tipo_risp= pid_server;

        if(mess.tipo== 1){
            risp.valore_letto= valore_risposta;

        }else if(mess.tipo== 2){
            risp.valore_letto= 0;
        }else{
            printf("[DATABASE]: Non ho capito che tipo di operazione devo fare\n");
        }

        result= msgsnd(parametri->id_coda_risposta, (void*)&risp, sizeof(messaggio_risposta)-sizeof(long), 0);

    }

    return NULL;

}