#include <sys/ipc.h>
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

int main(){

        pid_t pid;
        int ds_queue_control_proxy, ds_queue_proxy_gestori;
    
        //create queues
        int key_queue_control_proxy = IPC_PRIVATE;/* TBD: definire la chiave */
        int key_queue_proxy_gestori = IPC_PRIVATE;/* TBD: definire la chiave */
    
        ds_queue_control_proxy = msgget(key_queue_control_proxy, IPC_CREAT|0664);/* TBD: Creare la coda tra Controllore e Proxy */
        ds_queue_proxy_gestori = msgget(key_queue_proxy_gestori, IPC_CREAT|0664);/* TBD: Creare la coda tra Proxy e gestor */
    

        if(ds_queue_control_proxy<0){perror("[MAIN]: Errore nella creazione coda Proxy\n"); exit(1);}
        if(ds_queue_proxy_gestori<0){perror("[MAIN]: Errore nella creazione coda Gestori\n"); exit(1);}


        /* Creare le code di servizio per implementare la send sincrona */
        initServiceQueues();

        
        printf("[master] Code create...\n");
        printf("[master] ...........ds_queue_control_proxy: %d\n", ds_queue_control_proxy);
        printf("[master] ...........ds_queue_proxy_gestori: %d\n", ds_queue_proxy_gestori);
    

        int i;
        for (i=0; i<4; i++){
            /* TBD: Creare i 4 processi:
             * - Processo Controllore;
             * - Processo Proxy;
             * - Processo Gestore Arrivi
             * - Processo Gestore Partenze
             */
            pid= fork();

            if(pid==0){
                if(i==0){
                printf("[MAIN]: Creazione del processo Controllore...\n");
                controllore(ds_queue_control_proxy);
                exit(0);
                }
                if(i==1){
                printf("[MAIN]: Creazione del processo Proxy...\n");
                proxy(ds_queue_control_proxy, ds_queue_proxy_gestori);
                exit(0);
                }
                if(i==2){
                printf("[MAIN]: Creazione del processo Arrivi...\n");
                gestore_arrivi(ds_queue_proxy_gestori);
                exit(0);
                }
                if(i==3){
                printf("[MAIN]: Creazione del processo Partenze...\n");
                gestore_partenze(ds_queue_proxy_gestori);
                exit(0);
                }

            }else if(pid<0) {printf("[MAIN]: Errore nella Creazione dei Processi\n"); exit(1);}

        }
    
        for (i=0; i<4; i++){
            /*TBD: Attendere la terminazione dei processi creati in precedenza */
            wait(NULL);
            printf("[MAIN]: Terminazione dei processi, count: %d\n", i+1);
        }

        /* TBD: Deallocare le code create */
        msgctl(ds_queue_control_proxy, IPC_RMID, NULL);
        msgctl(ds_queue_proxy_gestori, IPC_RMID, NULL);
        removeServiceQueues();
         
        printf("[master] Rimozione code OK!\n");
    
        return 0;
}
