#ifndef __BARBERO_H__
#define __BARBERO_H__

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

class Barbero {
    public:
        Barbero ( int *_clientes, int *_clienteSiendoAtendido,
                  pthread_mutex_t *_mutex, pthread_cond_t *_dormir, 
                  pthread_cond_t *_sillas, pthread_cond_t *_subeSillon, 
                  pthread_cond_t *_ocupandoSillon);

        ~Barbero ();

        void printBarbero(char *s);
        
        void duerme(); 
        void cortaPelo(); 
        void finCorte();

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