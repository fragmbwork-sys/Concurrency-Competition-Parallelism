#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

void * thread_lettore(void *);
void * thread_scrittore(void *);

// Variabile globale
int id_coda_sensore;

int lettura(MonitorLS * m) {

    /* 
        NOTA: C'è un unico scrittore
    */
    pthread_mutex_lock(&m->mutex);

    int my_id = syscall(SYS_gettid);

    while(m->num_scritt>0){
        printf("[LETTURA]: C'e' uno scrittore, attendo\n");
        pthread_cond_wait(&m->VAR_LETT, &m->mutex);
    } 

    printf("[LETTURA]: Sono uscito dalla wait\n");
    m->num_lett++;

    pthread_mutex_unlock(&m->mutex);

     
    int val;
    val = m->valore;
    printf("[%d]Lettore: il valore letto = %d\n",my_id,val);
    // TODO: completare lettura

    pthread_mutex_lock(&m->mutex);

    m->num_lett--;

    if(m->num_lett==0){
        printf("[LETTURA]: Risveglio lo scrittore\n");
        pthread_cond_signal(&m->VAR_SCRITT);
    }

    pthread_mutex_unlock(&m->mutex);


        
    return val;
}

void scrittura(MonitorLS * m, int valore) {
    
    /* 
        NOTA: C'è un unico scrittore
    */
   pthread_mutex_lock(&m->mutex);
    
    int my_id = syscall(SYS_gettid);

    while(m->num_lett>0){
        printf("[SCRITTURA]: Ci sono gia' lettori, attendo\n");
        pthread_cond_wait(&m->VAR_SCRITT, &m->mutex);
    }

    printf("[SCRITTURA]: Sono uscito dalla wait\n");

    m->num_scritt++;

    pthread_mutex_unlock(&m->mutex);
    

    // TODO: completare scrittura
    m->valore = valore;
    printf("[%d]Scrittore: il valore scritto = %d\n",my_id,m->valore);

    pthread_mutex_lock(&m->mutex);

    m->num_scritt--;

    
    pthread_cond_broadcast(&m->VAR_LETT);


    pthread_mutex_unlock(&m->mutex);

    
    }

void main() {

	srand(time(NULL));

	int key_coda = ftok(FTOK_PATH, FTOK_CHAR);// TODO
    id_coda_sensore = msgget(key_coda, 0);// TODO

    printf("Avvio processo smistatore...\n");

    pthread_t scrittore;
    pthread_t lettori[LETTORI];

    MonitorLS * m = (MonitorLS*)malloc(sizeof(MonitorLS));//TODO

    // TODO: inizializzare variabili per la mutua esclusione
    m->valore=0;
    m->num_lett=0;
    m->num_scritt=0;
    pthread_cond_init(&m->VAR_LETT, NULL);
    pthread_cond_init(&m->VAR_SCRITT, NULL);
    pthread_mutex_init(&m->mutex, NULL);
    

    // TODO: creare thread scrittore
    int err= pthread_create(&scrittore, NULL, thread_scrittore, (void*)m);

    if(err){perror("[SMISTATORE]: Errore nella creazione del Thread Scrittore\n"); exit(1);}
    
    for(int i=0; i<3; i++) {
    	// TODO: creare thread lettori
        int err= pthread_create(&lettori[i], NULL, thread_lettore, (void *)m);
    	}

    // TODO: attendere la terminazione dei thread
    pthread_join(scrittore, NULL);
    printf("[SMISTATORE]: Terminazione del Thread Scrittore\n");

    for(int k=0; k<3; k++){
        pthread_join(lettori[k], NULL);
        printf("[SMISTATORE]: Terminazione del Thread Lettore n. %d\n", k+1);
    }

	// TODO: deallocare la struttura
    pthread_cond_destroy(&m->VAR_LETT);
    pthread_mutex_destroy(&m->mutex);

    free(m);
	


}

void * thread_lettore(void * x) {

    MonitorLS * ls = (MonitorLS*) x; //TODO: recupero parametri

    for(int i=0; i<10; i++) {

        sleep(1);

        // TODO: leggere il valore, moltiplicarlo per un intero tra 1 e 100 e stamparlo a video
        int valore_old= lettura(ls);

        int valore= valore_old*((rand()%100)+1);
        
        printf("Lettore: Stampo valore originale = %d e valore finale = %d\n", valore_old, valore);
    }

    pthread_exit(NULL);
}

void * thread_scrittore(void * x) {

    MonitorLS * ls = (MonitorLS*) x;// TODO: recupero parametri

    for(int i=0; i<10; i++) {

        printf("Smistatore: In attesa di messaggi...\n");

        // TODO: ricezione messaggio
        Messaggio msg;

        int result= msgrcv(id_coda_sensore, (void *)&msg, sizeof(Messaggio)-sizeof(long), TIPO, 0);

        if(result<0){perror("[SCRITTORE]: Errore nella ricezioned di un messaggio\n"); exit(1);}
                
        printf("Scrittore: Ricevuto valore = %d\n", msg.valore);

        // TODO: scrivere il messaggio nel monitor

        scrittura(ls, msg.valore);
            }

    pthread_exit(NULL);
}
