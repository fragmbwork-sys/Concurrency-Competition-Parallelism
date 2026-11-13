#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>

void main() {

    printf("Avvio processo sensore...\n");
    
    int key_coda = ftok(FTOK_PATH, FTOK_CHAR);//TODO
    int id_coda_sensore = msgget(key_coda, 0);// TODO

    srand(time(NULL));

    for(int i=0; i<10; i++) {

        // TODO: inviare interi casuali sulla coda
        int valore= rand() % (10 + 1 - 0) + 0; 

        Messaggio m;
        m.tipo= TIPO;
        m.valore= valore;

        int result= msgsnd(id_coda_sensore, (void *)&m, sizeof(Messaggio)-sizeof(long), 0);

        if(result<0){perror("[SENSORE]: Errore nell'invio di un messaggio\n"); exit(1);}       
        
        printf("[%d]Messaggio inviato: %d\n",getpid(),m.valore);
        
        sleep(1);
    }

}
