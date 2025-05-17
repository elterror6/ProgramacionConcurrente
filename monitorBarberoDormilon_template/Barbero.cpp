#include "Barbero.h"

Barbero::Barbero( int *_clientes, int *_clienteSiendoAtendido, /* Punteros que "apuntan" hacia las principales variables para resolver el problema
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

Barbero::~Barbero(){}

void Barbero::printBarbero(char *s) {
    printf("Barbero: -> %s\n", s);
}

void Barbero::duerme() {

    /* POR HACER:
     * Proteger poniendo cerrojo (para garantizar la exclusión mutua)
     * porque será necesario acceder a la variable compartida entre hilos que 
     * indica el número el número de clientes en la barbería en el momento actual.
     * En base al valor de esa variable (que tenéis que averiguar cuál es) el barbero 
     * actua de la siguiente manera.
     *    - Si ejecuta duerme() y no hay clientes, encontes el barbero indica que va a dormir ( printBarbero("voy a dormir") )
              En este caso, además, hay que suspender el hilo barbero con la variable de condición adecuada, hasta que un cliente "lo despierte"
     *        (utilizando la función pthread_cond_wait(...))
     *    - Si, por el contrario, al tratar de ejecutar "duerme()" sí hay clientes dentro de la barbería, entonces: 
     *        El barbero grita "Que pase el siguiente cliente!"
     *        Avisa (signal) a uno de los clientes "bloqueados" por la variable de condición que controla las sillas
     *        Y espera, suspendido, hasta que el cliente se siente/sube en el sillón de barbero.
     */
    pthread_mutex_lock( _mutex ); /* Poner cerrojo/lock */
    if ( *_clientes == 0 ) {
        printBarbero((char *) "Voy a dormir");
        while (*_clientes == 0) { /* Mientras no haya clientes en la barbería */
           pthread_cond_wait(_dormir, _mutex); /* Esperar a que un cliente despierte al barbero */
        }
        
    }
    else {
       printBarbero((char *)"Que pase el siguiente cliente!");
       pthread_cond_signal(_sillas);
       pthread_cond_wait(_subeSillon, _mutex);
    }
    pthread_mutex_unlock( _mutex ); /* Quitar cerrojo/lock */
}

void Barbero::cortaPelo() {
    /* POR HACER:
     * Proteger poniendo cerrojo (para garantizar la exclusión mutua)
     * porque será necesario indicar que se está cortando el pelo al "cliente que está 
     * siendo atendido" (variable compartida _clienteSendoAtendido).
     * Por último, liberar el cerrojo y hacer un sleep para suspender el hilo del barbero por
     * por ejemplo un entero aleatorio entre 1 y 5 segundos (que representa el tiempo que el
     * barbero está cortando el pelo).
    */
    srand(getpid());
    char aux[30];
    pthread_mutex_lock( _mutex );
    sprintf(aux, "Cortando pelo al cliente %d", *this->_clienteSiendoAtendido);
    printBarbero(aux);
    pthread_mutex_unlock( _mutex );
    sleep(rand() % 5 + 1);
}

void Barbero::finCorte() {
    pthread_mutex_lock(_mutex); /* Uso de un cerrojo para garantizar la exclusión mutua en el acceso y modificación de la variable compartida _clientes
                                y también al imprimir el id del cliente que acaba de ser atendido*/
        pthread_cond_signal(_ocupandoSillon); /* (sincronización) Para liberar el único sillón de barbero*/
        (*_clientes)--; /* Un cliente menos en la barbería*/
        printBarbero((char *) "Queda usted servido cliente "); 
        printf("%d\n", *this->_clienteSiendoAtendido); /* cliente recien atendido */
    pthread_mutex_unlock(_mutex);
}
