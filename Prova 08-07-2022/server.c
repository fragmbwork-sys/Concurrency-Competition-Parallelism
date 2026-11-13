//#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "header.h"

typedef struct {
    int value;
    //TODO: aggiunta delle variabili necessarie alla sincronizzazione
    // con lettori-scrittori con staravtion di entrambi
    pthread_cond_t VAR_SCRITT;
    pthread_cond_t VAR_LETT;

    pthread_mutex_t mutex;

    int num_lett;
    int num_scritt;

    int num_lett_wait;


} data_str;

typedef struct {
    int queue_res;
    int sender;
    data_str* data;
} reader_param;

int read_data(data_str* data) {
    int value;
    int my_id = syscall(SYS_gettid);
    
    //TODO: lettura valore da "data" attraverso
    // lettori-scrittori con starvation di entrambi
    pthread_mutex_lock(&data->mutex);

    while(data->num_scritt>0){
        data->num_lett_wait++;
        printf("[LETTORE]: C'e' gia' uno scrittore, attendo\n");
        pthread_cond_wait(&data->VAR_LETT, &data->mutex);
        data->num_lett_wait--;
    }

    data->num_lett++;

    pthread_mutex_unlock(&data->mutex);
    


    value = data->value;
    printf("[%d]Lettore: il valore letto = %d\n",my_id,value);
    


    pthread_mutex_lock(&data->mutex);

    data->num_lett--;

    if(data->num_lett==0){
        printf("[LETTORE]: Sono l'ultimo lettore, risveglio lo scrittore\n");
        pthread_cond_signal(&data->VAR_SCRITT);
    }

    pthread_mutex_unlock(&data->mutex);

    return value;
}

void write_data(data_str* data, int new_value) {
    //TODO: scrittura "new_value" in "data" attraverso
    // lettori-scrittori con starvation di entrambi
    int my_id = syscall(SYS_gettid);

    
    pthread_mutex_lock(&data->mutex);


    while(data->num_lett>0){
        printf("[SCRITTORE]: Ci sono gia' lettori, attendo\n");
        pthread_cond_wait(&data->VAR_SCRITT, &data->mutex);
    }

    data->num_scritt++;

    pthread_mutex_unlock(&data->mutex);
    


    data->value = new_value;
    printf("[%d]Scrittore: il valore scritto = %d\n",my_id,data->value);



    pthread_mutex_lock(&data->mutex);

    data->num_scritt--;

    if(data->num_lett_wait>0){
        printf("[SCRITTORE]: Risveglio tutti i lettori\n");
        pthread_cond_broadcast(&data->VAR_LETT);
    }

    pthread_mutex_unlock(&data->mutex);
    
}

void * updater(void* par) {
	int my_id = syscall(SYS_gettid);
    data_str* data = (data_str*) par;
    int new_value;
    int i;
    for (i = 0; i < NUM_UPDATES; i++) {
        new_value = rand()%10+1;
        printf("UPDATER[%d]: aggiornamento dato al valore: %d\n",my_id,new_value);
        write_data(data,new_value);
        sleep(rand()%3+1);
    }
    pthread_exit(NULL);
}

void * reader(void* par) {
    //TODO: ricostruzione parametro
    reader_param* param= (reader_param*) par;

	int my_id = syscall(SYS_gettid);
    res risp;
    //TODO: Costruzione risposta con prelievo valore dalla funzione read_data
    risp.sender= param->sender;
    risp.value= read_data(param->data);
    
    printf("READER_SERV[%d]: invio valore %d al client %ld\n",my_id,risp.value,risp.sender);
    //TODO: Invio risposta al reader client
    int result= msgsnd(param->queue_res, (void *)&risp, sizeof(res)-sizeof(long), 0);

    if(result<0){perror("[READER_SERVER]: Errore nell'invio di un messaggio di risposta\n"); exit(1);}
    
    printf("READER_SERV[%d]: valore %d al client %ld INVIATO\n",my_id,risp.value,risp.sender);

    pthread_exit(NULL);
}

int main() {

    int pid;
    key_t msg_req_key = ftok("./", 'f');//TODO: definire chiave
	key_t msg_res_key = ftok("./", 'g');//TODO: definire chiave

    //TODO: INIZIALIZZAZIONE CODE
    int queue_req= msgget(msg_req_key, 0);
	int queue_res= msgget(msg_res_key, 0);

    //TODO: inizializzazione struttura condivisa tra i thread
    data_str *data= (data_str*) malloc(sizeof(data_str));
    data->num_lett=0;
    data->num_scritt=0;
    data->value=0;
    data->num_lett_wait=0;

    //TODO: inizializzazione mutex e condition variables
    pthread_cond_init(&data->VAR_SCRITT, NULL);
    pthread_cond_init(&data->VAR_LETT, NULL);
    pthread_mutex_init(&data->mutex, NULL);

    pthread_t upd, rd[NUM_READS*NUM_READERS];

    reader_param p[NUM_READS*NUM_READERS];

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    srand(time(NULL));
    
    //TODO: creazione thread updater
    int err= pthread_create(&upd, &attr, updater, (void *)data);
    
    if(err){perror("[SERVER]: Errore nella creazione del processo Updater\n"); exit(1);}



    req msg;
    int i;
    for (i = 0; i < NUM_READS*NUM_READERS; i++) {
        //TODO: ricezione messaggio di richiesta
       int result= msgrcv(queue_req, (void *)&msg, sizeof(req)-sizeof(long), MESSAGGIO, 0);

       if(result<0){perror("[SERVER]: Errore nella ricezione di un messaggio di richiesta\n"); exit(1);}
       
       printf("READER:SERV: ricevuta richiesta di lettura da: %d\n",msg.pid);
        //TODO: preparazione parametro p[i]
        p[i].queue_res= queue_res;
        p[i].sender= msg.pid;
        p[i].data= data;
        
        //TODO: creazione thread reader
        int err= pthread_create(&rd[i], &attr, reader, (void *)&p[i]);

        if(err){perror("[SERVER]: Errore nella creazione del processo Reader\n"); exit(1);}


    }
    //TODO: join dei thread
    pthread_join(upd, NULL);
    printf("[SERVER]: Thread Server terminato.\n");


    for (i = 0; i < NUM_READS*NUM_READERS; i++){
         pthread_join(rd[i], NULL);
         printf("[SERVER]: Thread Reader n. %d terminato.\n", i+1);
    }
    

    //TODO: deallocazione memoria e rimozione coda
    shmctl(queue_res, IPC_RMID, NULL);
    shmctl(queue_req, IPC_RMID, NULL);

    pthread_cond_destroy(&data->VAR_SCRITT);
    pthread_cond_destroy(&data->VAR_LETT);
    pthread_mutex_destroy(&data->mutex);

    free(data);
    
    return 0;
}
