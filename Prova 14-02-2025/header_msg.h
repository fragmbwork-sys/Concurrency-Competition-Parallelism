#ifndef _HEADER_
#define _HEADER_

typedef int msg;

typedef struct {

    //TODO completare struttura messaggio
    long tipo;
    msg messaggio;

} messaggio_sensore;

typedef struct {

    //TODO completare struttura messaggio
    long tipo_coll;
    msg media;

} messaggio_collettore;


#define NUM_MESSAGGI_PER_SENSORE 5
#define NUM_SENSORI 3

#define TIPO_COLLETTORE 5

void sensore(int id_sensore, int id_queue_collettore);
void collettore(int id_queue_collettore, int id_queue_server);

#endif