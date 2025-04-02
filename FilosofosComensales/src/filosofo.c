#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <definitions.h>
/**
 * @file filosofo.c
 * @brief Programa que simula el comportamiento de un filósofo en el problema de los filósofos comensales.
 * 
 * Este programa simula el comportamiento de un filósofo que alterna entre pensar y comer. Utiliza buzones para sincronizarse con los demás filósofos.
 * 
 * @author Daniel Díaz García
 * @date 2025-04-02
 */

 /**
  * @brief Método que simula el comportamiento de un filósofo.
  */
void filosofo(char *filosofo, char *buzon_palillo_izq, char *buzon_palillo_der);

/**
 * @brief Método principal.
 * 
 * Este método recibe los argumentos de la línea de comandos y llama al método filosofo.
 * 
 * @param argc Número de argumentos.
 * @param argv Array de argumentos.
 *             'argv[1]': Número del filósofo.
 *             'argv[2]': Nombre del buzón del palillo izquierdo.
 *             'argv[3]': Nombre del buzón del palillo derecho.
 * @return int Código de salida del programa.
 */
int main (int argc, char *argv[]) {
    srand(getpid());
    filosofo (argv[1], argv[2], argv[3]);
    return EXIT_SUCCESS;
}

/**
 * @brief Método que simula el comportamiento de un filósofo.
 * 
 * Esta función se encarga de pensar y comer, utilizando los buzones de los palillos para sincronizarse con los demás filósofos.
 * 
 * @param filosofo Número del filósofo.
 * @param buzon_palillo_izq Nombre del buzón del palillo izquierdo.
 * @param buzon_palillo_der Nombre del buzón del palillo derecho.
 */
void filosofo (char *filosofo, char *buzon_palillo_izq, char *buzon_palillo_der) {
    mqd_t qHandlerMesa , qHandlerPalilloIzq, qHandlerPalilloDer;
    int n_filosofo;
    char buffer[64];

    n_filosofo = atoi(filosofo);
    // Abrimos el buzón de la mesa y los buzones de los palillos con permisos de lectura y escritura.
    qHandlerMesa = mq_open(BUZON_MESA, O_RDWR);
    qHandlerPalilloIzq = mq_open(buzon_palillo_izq, O_RDWR);
    qHandlerPalilloDer = mq_open(buzon_palillo_der, O_RDWR);

    while (1) {
        // Pensar
        printf("[FILOSOFO %d] Pensando ...\n", n_filosofo);
        sleep(rand() % MAX_TIME_PENSAR);
        // Solicitamos sentarnos a la mesa
        mq_receive(qHandlerMesa, buffer, sizeof(buffer), NULL);
        // Solicitamos los palillos
        mq_receive(qHandlerPalilloIzq, buffer, sizeof(buffer), NULL);
        mq_receive(qHandlerPalilloDer, buffer, sizeof(buffer), NULL);
        // Comer
        printf("[FILOSOFO %d] Comiendo ...\n", n_filosofo);
        sleep(rand() % MAX_TIME_COMER);
        // Devolvemos los palillos
        mq_send(qHandlerPalilloIzq, buffer, sizeof(buffer), 0);
        mq_send(qHandlerPalilloDer, buffer, sizeof(buffer), 0);
        // Dejamos la mesa y terminamos de comer
        mq_send(qHandlerMesa, buffer, sizeof(buffer), 0);
        printf("[FILOSOFO %d] Terminado de comer.\n", n_filosofo); 
    }
}
