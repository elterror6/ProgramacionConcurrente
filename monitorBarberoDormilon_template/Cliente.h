#ifndef __CLIENTE_H__
#define __CLIENTE_H__

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define NUM_SILLAS 3 //Máx. número de clientes esperando en las sillas de la sala de espera de la barbería

typedef enum {BARBERIA_LLENA, HAY_SITIO} respuesta;

class Cliente {
    public:

        Cliente ( int *_clientes, int *_clienteSiendoAtendido,
                  pthread_mutex_t *_mutex, pthread_cond_t *_dormir, 
                  pthread_cond_t *_sillas, pthread_cond_t *_subeSillon, 
                  pthread_cond_t *_ocupandoSillon);

        ~Cliente ();

        void printCliente(int i, char *s);

        int entraEnBarberia(int i);
        void irADarUnaVuelta(int i, int t);

    private:
        int *_clientes;
        int *_clienteSiendoAtendido;

        pthread_mutex_t *_mutex;
        pthread_cond_t *_dormir;
        pthread_cond_t *_sillas;
        pthread_cond_t *_subeSillon;
        pthread_cond_t *_ocupandoSillon;
};

#endif