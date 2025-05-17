#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "Barbero.h"
#include "Cliente.h"

#define NUM_CLIENTES 6 // Numero de clientes para la simulación

static void *barbero_func (void *arg);
static void *cliente_func (void *arg);

int main (int argc, char *argv[]){

    pthread_mutex_t mutex; /* para excl. mutua en los monitores */
    pthread_cond_t dormir, sillas, subeSillon, ocupandoSillon; /* para la sincronización */

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&dormir,NULL);
    pthread_cond_init(&sillas, NULL);
    pthread_cond_init(&subeSillon, NULL);
    pthread_cond_init(&ocupandoSillon, NULL);

    int clientes = 0; /* Numero de clientes (actualmente) en la barberia */
    int clienteSiendoAtendido = -1; /* Cliente que esta siendo atendido */

    Barbero *barbero = new Barbero(&clientes, &clienteSiendoAtendido, &mutex, &dormir, &sillas, &subeSillon, &ocupandoSillon); /* monitor para barbero */
    /* POR HACER */
    Cliente *cliente = new Cliente(&clientes, &clienteSiendoAtendido, &mutex, &dormir, &sillas, &subeSillon, &ocupandoSillon); /* monitor para clientes */

    int i;
    pthread_t id_clientes[NUM_CLIENTES], id_barbero;

    srand((int) getpid());

    /*Creacion de los hilos*/
    pthread_create(&id_barbero, NULL, barbero_func, barbero); 
    int r=rand() % 2; /*tiempo aleatorio para ir creando hilos "cliente"*/ 
    for (i=0; i<NUM_CLIENTES; i++){ 
        sleep(r+1);
        /* POR HACER */
        pthread_create(&id_clientes[i], NULL, cliente_func, cliente); /* crear hilo cliente */
    }
    /*Esperar terminación de los hilos "cliente"*/
    for (i=0; i<NUM_CLIENTES; i++) 
        pthread_join(id_clientes[i], NULL);

    pthread_cancel(id_barbero);     // solicita cancelación del barbero
    pthread_join(id_barbero, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&dormir);
    pthread_cond_destroy(&sillas);
    pthread_cond_destroy(&subeSillon);
    pthread_cond_destroy(&ocupandoSillon);

    delete barbero; 
    delete cliente;

    return EXIT_SUCCESS;
 }

static void *barbero_func (void *arg){
    Barbero *barbero = (Barbero *) arg; /*monitor para el barbero*/

    while (1){
        
        barbero->duerme(); /* El barbero duerme hasta que llega un cliente */
        barbero->cortaPelo(); /* El barbero corta el pelo al cliente */
        barbero->finCorte(); /* El barbero termina de cortar el pelo al cliente */

    }
    return (void *) true;
}

static void *cliente_func (void *arg){
    Cliente *cliente = (Cliente *) arg; /*monitor para el cliente*/ 
    int r=rand() % 15; /*tiempo aleatorio para ir a dar una vuelta*/ 
    pid_t i = gettid(); /*Identificamos al cliente con su TID (thread-identification)*/
    /* POR HACER
    * Un cliente intentará entrar en la barberia hasta que encuentre sitio en su interior.
    * De modo que, probará a entrar y si la barbería está llena (BARBERIA_LLENA) irá a dar una
    * vuelta por tiempo aleatorio (r+1) y lo intentará nuevamente (usar por ejemplo un bucle while...)
    */
    
    while (cliente->entraEnBarberia(i) == BARBERIA_LLENA){
        //ir a dar una vuelta
        cliente->irADarUnaVuelta(i, r+1);
    }
    return (void *) true;
}