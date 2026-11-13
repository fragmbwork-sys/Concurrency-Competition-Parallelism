#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "header.h"

void ProcessoP1(/* TBD: Completare il passaggio dei parametri */VettoreInput* v);
void ProcessoP2(/* TBD: Completare il passaggio dei parametri */VettoreInput* v);
void ProcessoP3(/* TBD: Completare il passaggio dei parametri */VettoreInput* v, MatriceParametri1 mat, VettoreIntermedio* vett);
void ProcessoP4(/* TBD: Completare il passaggio dei parametri */VettoreIntermedio* v, MatriceParametri2 matr);

int main() {

    srand(getpid());

    int id_shm= shmget(IPC_PRIVATE, sizeof(VettoreInput), IPC_CREAT|0664);

    VettoreInput * input = /* TBD: Allocare e inizializzare il vettore di input */shmat(id_shm, NULL, 0);

    if(input==(void*)-1){
        printf("[MAIN]: Errore nell'attach del VettoreInput\n");
        exit(1);
    }

    init_input(input);

    
    int id_shm2= shmget(IPC_PRIVATE, sizeof(VettoreIntermedio), IPC_CREAT|0664);

    VettoreIntermedio * intermedio = /* TBD: Allocare e inizializzare il vettore di risultati intermedio */shmat(id_shm2, NULL, 0);

    if(intermedio==(void*)-1){
        printf("[MAIN]: Errore nell'attach dell'Intermedio\n");
        exit(1);
    }

    init_intermedio(intermedio);


    MatriceParametri1 matrice_1;

    printf("Matrice 1: \n");

    for(int i=0; i<DIM3; i++) {

        for(int j=0; j<DIM4; j++) {

            matrice_1[i][j] = rand() % 10;

            printf("[%d][%d]=%d ", i, j, matrice_1[i][j]);
        }

        printf("\n");
    }

    printf("\n");



    MatriceParametri2 matrice_2;

    printf("Matrice 2: \n");

    for(int i=0; i<DIM6; i++) {

        for(int j=0; j<DIM7; j++) {

            matrice_2[i][j] = rand() % 10;

            printf("[%d][%d]=%d ", i, j, matrice_2[i][j]);
        }

        printf("\n");

    }

    printf("\n");



    /* TBD: Avviare i processi P1, P2, P3, P4 */
    pid_t pid1= fork();
    if(pid1==0){
        printf("[MAIN]: Creazione del Processo 1\n");
        ProcessoP1(input);
        exit(0);
    }

    pid_t pid2= fork();
    if(pid2==0){
        printf("[MAIN]: Creazione del Processo 2\n");
        ProcessoP2(input);
        exit(0);
    }

    pid_t pid3= fork();
    if(pid3==0){
        printf("[MAIN]: Creazione del Processo 3\n");
        ProcessoP3(input, matrice_1, intermedio);
        exit(0);
    }

    pid_t pid4= fork();
    if(pid4==0){
        printf("[MAIN]: Creazione del Processo 4\n");
        ProcessoP4(intermedio, matrice_2);
        exit(0);
    }


    /* TBD: Attendere la terminazione dei processi P1, P2, P3, P4 */
    for(int k=0; k<4; k++){

        printf("[MAIN]: Aspetto la terminazione del processo n- %d\n", k);
        wait(NULL);
    }


    /* TBD: De-inizializzare e deallocare i vettori */
    remove_input(input);
    remove_intermedio(intermedio);
    shmctl(id_shm, IPC_RMID, 0);
    shmctl(id_shm2, IPC_RMID, 0);



}


void ProcessoP1(/* TBD: Completare il passaggio dei parametri */VettoreInput* v) {

    for(int i=0; i<3; i++) {

        printf("[P1] Inserimento vettore input: ");

        int val[DIM1];

        for(int j=0; j<DIM1; j++) {

            val[j] = rand() % 10;

            printf("[%d]=%d ", j, val[j]);
        }

        printf("\n");

        inserisci_input_primo_vettore(/* TBD: Completare il passaggio dei parametri */v, val);

        sleep(1);
    }

}

void ProcessoP2(/* TBD: Completare il passaggio dei parametri */VettoreInput* v) {

    for(int i=0; i<3; i++) {

        printf("[P2] Inserimento vettore input: ");

        int val[DIM2];

        for(int j=0; j<DIM2; j++) {

            val[j] = rand() % 10;

            printf("[%d]=%d ", DIM1+j, val[j]);
        }

        printf("\n");

        inserisci_input_secondo_vettore(/* TBD: Completare il passaggio dei parametri */v, val);

        sleep(1);
    }
}

void ProcessoP3(/* TBD: Completare il passaggio dei parametri */VettoreInput* v, MatriceParametri1 matrice_1, VettoreIntermedio* vett) {

    for(int i=0; i<3; i++) {

        int val[DIM1+DIM2];

        preleva_input(/* TBD: Completare il passaggio dei parametri */v, val);

        printf("[P3] Prelevamento vettore input: ");

        for(int j=0; j<DIM1+DIM2; j++) {

            printf("[%d]=%d ", j, val[j]);
        }

        printf("\n");



        int risultato[DIM5];

        for(int i=0; i<DIM3; i++) {

            risultato[i] = prodotto_scalare(/* TBD: Completare il passaggio dei parametri */val, matrice_1[i], DIM4);
        
        }


        printf("[P3] Inserimento vettore intermedio: ");

        for(int j=0; j<DIM5; j++) {

            printf("[%d]=%d ", j, risultato[j]);
        }

        printf("\n");

        inserisci_intermedio_vettore(/* TBD: Completare il passaggio dei parametri */vett, risultato);

        sleep(1);
    }

}

void ProcessoP4(VettoreIntermedio * intermedio, MatriceParametri2 matrice_2) {

    for(int i=0; i<3; i++) {

        int val[DIM5];

        preleva_intermedio(intermedio, val);

        printf("[P4] Prelevamento vettore intermedio: ");

        for(int j=0; j<DIM5; j++) {

            printf("[%d]=%d ", j, val[j]);
        }

        printf("\n");


        Output risultato;

        for(int i=0; i<DIM6; i++) {

            risultato[i] = prodotto_scalare(val, matrice_2[i], DIM7);
        
        }



        printf("[P4] Risultato: ");

        for(int j=0; j<DIM8; j++) {

            printf("[%d]=%d ", j, risultato[j]);
        }

        printf("\n");

    }


}

