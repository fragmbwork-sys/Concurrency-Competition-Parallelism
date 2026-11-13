#include "header.h"

int prodotto_scalare(int *v1, int *v2, int n) {

    int sum = 0;

    for(int i=0; i<n; i++) {

        sum += v1[i] * v2[i];
    }

    return sum;
}



void init_input(VettoreInput *v) {

    /* TBD: Completare l'inizializzazione */
    init_monitor(&(v->m), 2);
    v->nelem1= DIM1;
    v->nelem2= DIM2;
    for(int i=0; i<DIM1; i++){
        v->primo_vettore[i]= 0;
    }
    for(int i=0; i<DIM2; i++){
        v->secondo_vettore[i]= 0;
    }
}

void init_intermedio(VettoreIntermedio *v) {

    /* TBD: Completare l'inizializzazione */
    init_monitor(&(v->m2), 2);
    v->nelem= DIM5;
    for(int i=0; i<DIM5; i++){
        v->vettore[i]= 0;
    }
}



void remove_input(VettoreInput *v) {

    /* TBD: Completare la de-inizializzazione */
    remove_monitor(&(v->m));
}

void remove_intermedio(VettoreIntermedio *v) {

    /* TBD: Completare la de-inizializzazione */
    remove_monitor(&(v->m2));
}

void inserisci_input_primo_vettore(VettoreInput *v, int *val) {

    /* TBD: Completare la sincronizzazione */
    enter_monitor(&(v->m));

    
    if(v->nelem1<DIM1){

        printf("[INSERISCI_PRIMO]: Non posso inserire niente perche' il vettore e' pieno\n");
        wait_condition(&(v->m), VAR_PROD);
    }

    printf("[INSERISCI_PRIMO]: Esco dalla wait...\n");

    for(int i=0; i<DIM1; i++) {

        v->primo_vettore[i] = val[i];
        v->nelem1--;
    }

    signal_condition(&(v->m), VAR_CONS);

    leave_monitor(&(v->m));

}

void inserisci_input_secondo_vettore(VettoreInput *v, int *val) {

    /* TBD: Completare la sincronizzazione */

    enter_monitor(&v->m);


    if(v->nelem2<DIM2){
        printf("[INSERISCI_SECONDO]: Non posso produrre perché il vettore e' pieno\n");
        wait_condition(&(v->m), VAR_PROD);
    }

    printf("[INSERISCI_SECONDO]: Esco dalla wait...\n");

    for(int i=0; i<DIM2; i++) {

        v->secondo_vettore[i] = val[i];
        v->nelem2--;
    }

    signal_condition(&(v->m), VAR_CONS);

    leave_monitor(&v->m);

}

void preleva_input(VettoreInput *v, int *val) {

    /* TBD: Completare la sincronizzazione */

    enter_monitor(&(v->m));

    if(v->nelem1>0 || v->nelem2>0){
        printf("[PRELEVA_INPUT]: Non posso ancora prelevare perche' non c'e' niente\n");
        wait_condition(&v->m, VAR_CONS);
    }

    printf("[PRELEVA_INPUT]: Esco dalla wait\n");

    for(int i=0; i<DIM1; i++) {

        val[i] = v->primo_vettore[i];
        v->nelem1++;
    }



    /*if(v->nelem2>0){
        printf("[PRELEVA_INPUT_2]: Non posso ancora prelevare perche' non c'e' niente\n");
        wait_condition(&v->m, VAR_CONS);
    }

    printf("[PRELEVA_INPUT_2]: Esco dalla seconda wait\n");*/

    for(int i=0; i<DIM2; i++) {

        val[i+DIM1] = v->secondo_vettore[i];
        v->nelem2++;
    }

    signal_condition(&v->m, VAR_PROD);
    signal_condition(&v->m, VAR_PROD);

    leave_monitor(&v->m);

}

void inserisci_intermedio_vettore(VettoreIntermedio *v, int *val) {

    /* TBD: Completare la sincronizzazione */

    enter_monitor(&v->m2);

    if(v->nelem<DIM5){
        printf("[INSERISCI_INTERMEDIO]: Non posso ancora produrre perche' non c'e' spazio\n");
        wait_condition(&v->m2, VAR_PROD);
    }

    printf("[INSERISCI_INTERMEDIO]: Sono uscito dalla wait\n");

    for(int i=0; i<DIM5; i++) {

        v->vettore[i] = val[i];
        v->nelem--;
    }

    signal_condition(&v->m2, VAR_CONS);

    leave_monitor(&v->m2);

}

void preleva_intermedio(VettoreIntermedio *v, int *val) {

    /* TBD: Completare la sincronizzazione */

    enter_monitor(&v->m2);

    if(v->nelem>0){
        printf("[PRELEVA_INTERMEDIO]: Non posso ancora prelevare perche' non ci sono abbastanza elementi\n");
        wait_condition(&v->m2, VAR_CONS);
    }

    printf("[PRELEVA_INTERMEDIO]: Sono uscito dalla wait\n");

    for(int i=0; i<DIM5; i++) {

        val[i] = v->vettore[i];
        v->nelem++;
    }

    signal_condition(&v->m2, VAR_PROD);

    leave_monitor(&v->m2);

}

