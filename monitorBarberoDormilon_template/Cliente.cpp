#include "Cliente.h"

Cliente::Cliente(int *_clientes, int *_clienteSiendoAtendido, /* Punteros que "apuntan" hacia las principales variables para resolver el problema
                                                                del hilo padre Manager...*/
        pthread_mutex_t *_mutex, pthread_cond_t *_dormir, 
        pthread_cond_t *_sillas, pthread_cond_t *_subeSillon, 
        pthread_cond_t *_ocupandoSillon){
    this->_clientes = _clientes;
    this->_clienteSiendoAtendido = _clienteSiendoAtendido;
    this->_mutex = _mutex;
    this->_dormir = _dormir;
    this->_sillas = _sillas;
    this->_subeSillon = _subeSillon;
    this->_ocupandoSillon = _ocupandoSillon;
}

Cliente::~Cliente(){}

void Cliente::printCliente(int i, char *s) {
    printf("Cliente: %d -> %s\n", i,s);
}

void Cliente::irADarUnaVuelta(int i, int t) {
    printCliente(i,(char *) "Uy, está lleno, voy a dar una vuelta"); 
    printf(" durante (%d seg.)\n",t);
    sleep(t);
    printCliente(i,(char *)"Vengo de dar una vuelta");
}

int Cliente::entraEnBarberia(int i) {
    /* POR HACER:
     * El cliente entra en la barbería y lo imprime por consola (printCliente)
     * Hay que proteger poniendo el cerrojo (para garantizar la exclusión mutua)
     * porque será necesario acceder y/o modificar las variables compartidas entre hilos que
     * indican el "número de clientes dentro de la barbería", así como el "cliente siendo atendido".
     * Con el cerrojo activado... el cliente se encuentra ante dos situaciones principales:
     *      - Que no queda espacio en la barbería (>=NUM_SILLAS) en ese caso, se quita el bloqueo en el mutex
     *        y retorna (return) el estado de BARBERIA_LLENA. 
     *      - Que hay hueco/sillas libres dentro. En ese caso, se actualiza/incrementa el "núm. de clientes en la barbería" y esto conduce a 
     *        dos nuevas situaciones:
     *          - 1ª situacion. Estamos ante el primer cliente que entra en la barbería (hay que desperar al barbero que está durmiento con el corresponiente signal).
     *          - 2ª situación. No es el primer cliente, por tanto, debe esperar en una silla (bloquearse con un wait sobre la correspondiente variable de condición).
     *              Cuando sea desbloqueado (es porque el barbero le va a cortar el pelo y, por tanto, éste (el barbero) queda esperando a que se "suba en el sillón")
     *              o, dicho de otra manera, el cliente tendrá que hacer un signal a (_subeSillon); 
     *
     *           Depués, tanto para la situación 1ª como para la 2ª, el cliente debe indicar que él es el "cliente que está siendo atendido",
     *              modificando la variable compartida correspondiente y, por último, la operación wait correspondiente para esperar/suspenderse (mientras le cortan 
     *              el pelo y ocupa el sillón).            
     */ 

    pthread_mutex_lock( _mutex );
        printCliente(i,(char *)"Entra en la barbería.");
        if ( *_clientes >= NUM_SILLAS) {  //No queda epacio
              pthread_mutex_unlock(_mutex);
              return BARBERIA_LLENA;
        } 
        else {   // hay hueco/sillas
            *_clientes = *_clientes + 1; // Incrementar el número de clientes
            if (*_clientes == 1) { // Primer cliente (paso a despertar al barbero)
                pthread_cond_signal(_dormir);
            } else { // no
                pthread_cond_wait(_sillas, _mutex); // Esperar en la silla
                pthread_cond_signal(_subeSillon); // Avisar al barbero que se sube al sillón
            }
            
            *_clienteSiendoAtendido = i; // Indicar que soy el cliente que está siendo atendido
            pthread_cond_wait(_ocupandoSillon,_mutex);
    pthread_mutex_unlock(_mutex);
        }
        printCliente(i, (char *)"¡Gracias por el corte!");
    return HAY_SITIO;
}