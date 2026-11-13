#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "teatro.h"


int main(){
    
    int i;
    
    pthread_t client_threads[10];
    pthread_t viewer_threads[10];
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    
    srand(time(NULL)*getpid());
    
    posto_teatro *p = /* TODO: allocare memoria per il buffer condiviso */malloc(sizeof(posto_teatro));
    LettScritt *m = /* TODO: allocare memoria per il monitor */malloc(sizeof(LettScritt));
    
    /* TODO: inizializzazione variabili da usare durante la sincronizzazione */
    p->ID=0;
    p->tipo=0;
    m->num_lett=0;
    m->num_scritt=0;
    m->num_lett_wait=0;
    m->num_scritt_wait=0;
    pthread_mutex_init(&m->mutex, NULL);
    pthread_cond_init(&m->VAR_SCRITT, NULL);
    pthread_cond_init(&m->VAR_SCRITT, NULL);
    m->p= p;

    
    printf("[teatro] Creazione dei thread client e viewer\n");
    
    //creazione dei threads client, totale scritture 2* 10 = 20
    for(i=0;i<2;i++){
        /* TODO: creare i thread clients */
        pthread_create(&client_threads[i], &attr, clientT, (void*)m);
    }
    
    //creazione dei threads viewer, totali letture 5*4 = 20
    for(i=0;i<5;i++){
        /* TODO: creare i thread viewers */
        pthread_create(&viewer_threads[i], &attr, viewerT, (void*)m);
    }
    
    //join dei threads clients
    for(i=0;i<2;i++){
        /* TODO: attendere la terminazione dei thread clients */
        pthread_join(client_threads[i], NULL);
        printf("client_threads %d terminato\n",i+1);
    }
    
    //join dei threads viewers
    for(i=0;i<5;i++){
        /* TODO: attendere la terminazione dei thread viewers */
        pthread_join(viewer_threads[i], NULL);
        printf("viewer_threads %d terminato\n",i+1);
    }
    
    
    //deallocazione monitor
    /* TODO: deallocazione variabili usate durante la sincronizzazione */
    pthread_cond_destroy(&m->VAR_LETT);
    pthread_cond_destroy(&m->VAR_SCRITT);
    pthread_mutex_destroy(&m->mutex);

    free(p);
    free(m);
    
    pthread_exit(NULL);
}

