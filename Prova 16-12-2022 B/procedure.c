#include "procedure.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <time.h>


int stampa_risultato(MonitorRisultati * ls) {

	int risultato;

	// TODO: consumo il valore
	enter_monitor(&ls->m);

	if(ls->nelem==0){
		printf("[CONSUMATORE]: Non ci sono elementi da consumare, attendo...\n");
		wait_condition(&ls->m, VARCOND_CONSUMATORI);
	}

	printf("[CONSUMATORE: Sono uscito dalla wait\n");

	risultato= ls->buffer[ls->testa];
	ls->nelem--;
	ls->testa= (ls->testa+1)%DIM;

	printf("[CONSUMATORE]: Ho consumato il valore: %d\n", risultato);

	signal_condition(&ls->m, VARCOND_PRODUTTORI);

	leave_monitor(&ls->m);
	
	return risultato;
		
}


void inserisci_risultato(MonitorRisultati * ls, int valore) {

	// TODO: produco il valore
	enter_monitor(&ls->m);

	if(ls->nelem==DIM){
		printf("[INSERISCI]: Coda piena, attendo...\n");
		wait_condition(&ls->m, VARCOND_PRODUTTORI);
	}

	printf("[INSERISCI]: Sono uscito dall'attesa\n");

	ls->buffer[ls->coda]= valore;
	ls->nelem++;
	ls->coda= (ls->coda+1)%DIM;

	printf("[INSERISCI]: Ho inserito nella coda il valore: %d\n", valore);

	signal_condition(&ls->m, VARCOND_CONSUMATORI);

	leave_monitor(&ls->m);
	


}

void filtro(MonitorRisultati * ls, int coda){
	Messaggio m;
	int valore = 0;
	for (int i = 0 ; i < 4; i++){
		// TODO: filtro i valori prelevati dalla mailbox: se sono pari, li inserisco, altrimenti inserisco 0
			
	int result= msgrcv(coda, (void *)&m, sizeof(Messaggio)-sizeof(long), MESSAGGIO, 0);

	if(result<0){perror("[FILTRO]: Errore nella ricezione di un messaggio\n"); exit(1);}

		if((m.valore)%2==0){
			inserisci_risultato(ls, m.valore);
		}else{
			inserisci_risultato(ls, 0);
			}
	
	
	}

}
void generazione(int coda) {
	srand(getpid()*time(NULL));
	Messaggio m;
	sleep(2);
	for (int i = 0 ; i < 4; i++) {
		
		// TODO: genero dei numeri casuali tra 1 e 20 e li invio alla mailbox
		int valore= (rand()%20)+1;
		m.tipo= MESSAGGIO;
		m.valore= valore;

		int result= msgsnd(coda, (void *)&m, sizeof(Messaggio)-sizeof(long), 0);

		if(result<0){perror("[GENERAZIONE]: Errore nell'invio di un messaggio alla mailbox\n"); exit(1);}		
	
	}
}
void stampa(MonitorRisultati * ls) {
	int val;
	for (int i = 0 ; i < 4; i++) {
		
		// TODO: stampo il valore appena consumato
	val= stampa_risultato(ls);

	printf("[STAMPA]: Valore stampato: %d\n", val);
	
	}
}
