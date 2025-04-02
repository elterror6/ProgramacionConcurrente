#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <definitions.h>
#include <signal.h>
/**
 * @file manager.c
 * @brief Programa que lanza los procesos de los filósofos y gestiona la creación y eliminación de los buzones.
 * 
 * Este programa crea un buzón para la mesa y un buzón para cada palillo. Luego lanza los procesos de los filósofos, que se encargan de pensar y comer.
 * Al finalizar, libera los recursos y espera a que los procesos terminen.
 * 
 * @author Daniel Díaz García
 * @date 2025-04-02
 */

 /**
  * @brief Array de identificadores de procesos de los filósofos.
  */
pid_t pids[FILOSOFOS];
/**
 * @brief Identificador del buzón de la mesa.
 */
mqd_t qHandlerMesa;
/**
 * @brief Array de identificadores de los buzones de los palillos.
 */
mqd_t qHandlerPalillos[FILOSOFOS];

/**
 * @brief Función que maneja la señal de interrupción (Ctrl+C).
 * 
 * @param signal Número de la señal recibida.
 */
void controlador (int signal);
/**
 * @brief Función que libera los recursos utilizados por el programa.
 */
void liberarRecursos ();
/**
 * @brief Función que finaliza los procesos de los filósofos.
 */
void finalizarProcesos ();
/**
 * @brief Función que lanza los procesos de los filósofos.
 */
void lanzarProcesos ();
/**
 * @brief Función que crea el buzón de la mesa y los buzones de los palillos.
 * 
 * @param mqAttr Atributos del buzón.
 * @param buzon_mesa Identificador del buzón de la mesa.
 * @param buffer Buffer para enviar mensajes.
 */
void crearBuzonMesa (struct mq_attr *mqAttr, mqd_t *buzon_mesa ,char *buffer);
/**
 * @brief Función que crea los buzones de los palillos.
 * 
 * @param mqAttr Atributos del buzón.
 * @param buzon_palillos Array de identificadores de los buzones de los palillos.
 * @param buffer Buffer para enviar mensajes.
 */
void crearBuzonesPalillos (struct mq_attr *mqAttr, mqd_t *buzon_palillos, char *buffer);
/**
 * @brief Función que instala el manejador de la señal de interrupción (Ctrl+C).
 */
void instalarManejadorSignal ();
/**
 * @brief Función que espera a que los procesos de los filósofos terminen.
 */
void esperarProcesos ();

/**
 * @brief Función principal del programa.
 * 
 * @param argc Número de argumentos.
 * @param argv Array de argumentos.
 * @return int Código de salida del programa.
 */
int main (int argc, char *argv[]) {
    struct mq_attr mqAttr;
    char buffer[64];
    // Reservamos memoria para el array de pids
    memset(pids, 0, sizeof(pid_t) * FILOSOFOS);

    mqAttr.mq_maxmsg = (FILOSOFOS -1); mqAttr.mq_msgsize = 64;
    
    instalarManejadorSignal();

    // Creamos los buzones de la mesa y los palillos
    crearBuzonMesa(&mqAttr, &qHandlerMesa, buffer);
    crearBuzonesPalillos(&mqAttr, qHandlerPalillos, buffer);

    // Lanzamos los procesos de los filósofos
    lanzarProcesos();

    // Esperamos a que los procesos terminen en orden (no van a terminar nunca en este caso)
    esperarProcesos();
    printf("\nFin del programa.\n");

    return EXIT_SUCCESS;
}

/**
 * @brief Función que instala el manejador de la señal de interrupción.
 * 
 * Esta función se encarga de gestionar la señal de interrupción (Ctrl+C).
 */
void instalarManejadorSignal () {
    if (signal(SIGINT,controlador) == SIG_ERR) {
        fprintf(stderr, "Terminación abrupta del programa.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Función que maneja la señal de interrupción (Ctrl+C).
 * 
 * Esta función se encarga de liberar los recursos y finalizar los procesos al recibir la señal de interrupción.
 * 
 * @param signal Número de la señal recibida.
 */
void controlador (int signal) {
    liberarRecursos(); finalizarProcesos();
    printf("Recursos liberados y procesos terminados. (Ctrl+C)\n");
    exit(EXIT_SUCCESS);
}

/**
 * @brief Función que crea el buzón de la mesa.
 * 
 * Esta función crea el buzón de la mesa y envía un mensaje a cada filósofo para que se registre en el buzón.
 * Se envian solo FILOSOFOS-1 mensajes, para evitar interbloqueos.
 * 
 * @param mqAttr Atributos del buzón.
 * @param buzon_mesa Identificador del buzón de la mesa.
 * @param buffer Buffer para enviar mensajes.
 */
void crearBuzonMesa (struct mq_attr *mqAttr, mqd_t *buzon_mesa, char *buffer) {
    *buzon_mesa = mq_open(BUZON_MESA, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR, mqAttr);
    for (int i = 0; i < (FILOSOFOS-1); i++) {
        mq_send(*buzon_mesa, buffer, strlen(buffer), 1);
    }
}

/**
 * @brief Función que crea los buzones de los palillos.
 * 
 * Esta función crea un buzón para cada palillo y envía un mensaje a cada filósofo para que se registre en el buzón.
 * 
 * @param mqAttr Atributos del buzón.
 * @param buzon_palillos Array de identificadores de los buzones de los palillos.
 * @param buffer Buffer para enviar mensajes.
 */
void crearBuzonesPalillos (struct mq_attr *mqAttr, mqd_t *buzon_palillos, char *buffer) {
    char caux[30];
    mqAttr->mq_maxmsg = 1;
    
    for (int i = 0; i < FILOSOFOS; i++) {
        sprintf(caux, "%s%d",BUZON_PALILLO, i);
        buzon_palillos[i] = mq_open(caux, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR, mqAttr);
        mq_send(buzon_palillos[i], buffer, sizeof(buffer), 1);
    }
}

/**
 * @brief Función que libera los recursos utilizados por el programa.
 * 
 * Esta función cierra y elimina los buzones de la mesa y de los palillos.
 */
void liberarRecursos () {
    char caux[30];
    printf ("\nLiberando buzones...\n");
    mq_close(qHandlerMesa); mq_unlink(BUZON_MESA);
    for (int i = 0; i < FILOSOFOS; i++) {
        sprintf(caux, "%s%d", BUZON_PALILLO, i);
        mq_close(qHandlerPalillos[i]); mq_unlink(caux);
    }   
}

/**
 * @brief Función que finaliza los procesos de los filósofos.
 * 
 * Esta función envía una señal de terminación a cada proceso de filósofo.
 */
void finalizarProcesos () {
    printf ("-----------Finalizando-----------\n");
    for (int i = 0; i < FILOSOFOS; i++) {
        if (pids[i]) {
            printf("[%s %d] Finalizando proceso...\n", "FILOSOFO", pids[i]);
            kill(pids[i], SIGTERM);
        }
    }
}

/**
 * @brief Función que lanza los procesos de los filósofos.
 * 
 * Esta función crea un proceso hijo para cada filósofo y ejecuta el programa del filósofo.
 * 
 */
void lanzarProcesos () {
    char buzon_palillo_izq[30], buzon_palillo_der[30], filosofo[3];

    for (int i = 0; i < FILOSOFOS; i++) {
        if ((pids[i] = fork()) == 0) {
            sprintf(filosofo, "%d", i);
            sprintf(buzon_palillo_izq, "%s%d", BUZON_PALILLO, i);
            sprintf(buzon_palillo_der, "%s%d", BUZON_PALILLO, (i+1)%FILOSOFOS);
            execl("./exec/filosofo", "filosofo", filosofo, buzon_palillo_izq, buzon_palillo_der, NULL);
        }
    }
}

/**
 * @brief Función que espera a que los procesos de los filósofos terminen.
 * 
 * Esta función espera a que cada proceso de filósofo termine y libera los recursos utilizados.
 */
void esperarProcesos () {
    for (int i = 0; i < FILOSOFOS; i++) {
        waitpid(pids[i], 0, 0);
    }
    finalizarProcesos(); liberarRecursos();
}