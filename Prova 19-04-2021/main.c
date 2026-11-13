#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "auth.h"

int main() {

    printf("Creazione code di messaggi\n");

    int id_coda_auth_req = msgget(IPC_PRIVATE, IPC_CREAT|0664);/* TBD */

    int id_coda_auth_resp = msgget(IPC_PRIVATE, IPC_CREAT|0664);/* TBD */
    
    int id_coda_elab_req = msgget(IPC_PRIVATE, IPC_CREAT|0664);/* TBD */
    
    int id_coda_elab_resp = msgget(IPC_PRIVATE, IPC_CREAT|0664);/* TBD */


    if(id_coda_auth_req<0){printf("[MAIN]: Errore nella creazione della coda auth_req\n"); exit(1);}

    if(id_coda_auth_resp<0){printf("[MAIN]: Errore nella creazione della coda auth_resp\n"); exit(1);}

    if(id_coda_elab_req<0){printf("[MAIN]: Errore nella creazione della coda elab_req\n"); exit(1);}

    if(id_coda_elab_resp<0){printf("[MAIN]: Errore nella creazione della coda elab_resp\n"); exit(1);}


    pid_t pid;


    printf("[MAIN]: Avvio processo autenticatore\n");

    /* TBD: Avvio processo (funzione Auth) */
    pid= fork(); 
    if(pid==0){
        //printf("[MAIN]: Creazione processo Autenticatore...\n");
        Auth(id_coda_auth_req, id_coda_auth_resp);
        exit(0);
    }else if(pid<0){
        printf("[MAIN]: Errore nella creazione del processo Autenticatore\n");
        exit(1);
    }


    printf("[MAIN]: Avvio processo server\n");

    /* TBD: Avvio processo (funzione Server) */
    pid_t pid2= fork(); 
    if(pid2==0){
        //printf("[MAIN]: Creazione processo Autenticatore...\n");
        Server(id_coda_elab_req, id_coda_elab_resp, id_coda_auth_req, id_coda_auth_resp);
        exit(0);
    }else if(pid2<0){
        printf("[MAIN]: Errore nella creazione del processo Server\n");
        exit(1);
    }
	


    printf("[MAIN]: Avvio processo client autenticato\n");

    /* TBD: Avvio processo (funzione ClientAuth) */
    pid_t pid3= fork(); 
    if(pid3==0){
        //printf("[MAIN]: Creazione processo Autenticatore...\n");
        ClientAuth( id_coda_auth_req, id_coda_auth_resp, id_coda_elab_req, id_coda_elab_resp);
        exit(0);
    }else if(pid3<0){
        printf("[MAIN]: Errore nella creazione del processo Client Autorizzato\n");
        exit(1);
    }
    
    

    printf("[MAIN]: Avvio processo client non-autenticato\n");

    /* TBD: Avvio processo (funzione ClientNonAuth) */
    pid_t pid4= fork(); 
    if(pid4==0){
        //printf("[MAIN]: Creazione processo Autenticatore...\n");
        ClientNonAuth(id_coda_elab_req, id_coda_elab_resp);
        exit(0);
    }else if(pid4<0){
        printf("[MAIN]: Errore nella creazione del processo Client non Autorizzato\n");
        exit(1);
    }
    


    printf("[MAIN]: Attesa terminazione processi\n");

    /* TBD: Attesa terminazione */
    for(int k=0; k<4; k++){
        wait(NULL);
        printf("[MAIN]: Terminazione del processo n. %d\n", k+1);
    }
    


    printf("[MAIN]: Rimozione code di messaggi\n");

    /* TBD: Rimozione code */ 
    msgctl(id_coda_auth_req,  IPC_RMID,  NULL);
    msgctl(id_coda_auth_resp,  IPC_RMID,  NULL);
    msgctl(id_coda_elab_req,  IPC_RMID,  NULL);
    msgctl(id_coda_elab_resp,  IPC_RMID,  NULL);

    return 0;
}
