/**
 * @file manager.c
 * @brief Programa manager problema puente de un solo carril
 * 
 * Este programa realiza el problema del puente de un solo carril.
 * 
 * @author Daniel Diaz Garcia
 * @date 2025-03-10
 */
#include <memoriaI.h>
#include <semaforoI.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @def COCHES
 * @brief Maximo de coches
 * 
 * Indica el numero maximo de procesos coche que se van a crear.
 */
#define COCHES 30
/**
 * @def MAX_T_LANZAR
 * @brief Maximo tiempo de lanzado
 * 
 * Indica el tiempo maximo que tarda en lanzar un coche al puente.
 */
#define MAX_T_LANZAR 3 

/**
 * @def PUENTE
 * @brief Identificador de Puente
 * 
 * Indica el identificador del puente.
 */
#define PUENTE "bridge"
/**
 * @def MUTEXN
 * @brief Identificador de semaforo norte
 * 
 * Indica el identificador del semaforo del norte del puente.
 */
#define MUTEXN "mutexn"
/**
 * @def MUTEXS
 * @brief Identificador de semaforo sur
 * 
 * Indica el identificador del semaforo del sur del puente.
 */
#define MUTEXS "mutexs"

/**
 * @def COCHESN
 * @brief Identificador de conches norte
 * 
 * Indica el identificador de la variable de memoria compartida que representa el numero de coches que hay en el norte.
 */
#define COCHESN "cn"
/**
 * @def COCHESS
 * @brief Identificador de coches sur
 * 
 * Indica el identificador de la variable de memoria compartida que representa el numero de coches que hay en el sur.
 */
#define COCHESS "cs"

// Array de procesos coche
pid_t pids[COCHES];

// Declaracion de metodos
void liberarRecursos(); void finalizarProcesos(); void controlador(int senial);
/**
 * @brief Metodo main
 * 
 * Metodo principal del problema del puente de un solo carril.
 */
int main (int argc, char *argv[]) {
    pid_t pid_hijo;
    int i;

    srand( (int) getpid() );

    // Crear semaforos y segmentos de memoria compartida
    crear_sem(PUENTE, 1); crear_sem(MUTEXN, 1); crear_sem(MUTEXS, 1);
    crear_var(COCHESN, 0); crear_var(COCHESS, 0);

    if (signal(SIGINT, controlador) == SIG_ERR) {
        printf(stderr,"Abrupt termination.\n"); exit (EXIT_FAILURE);
    }

    for (i = 0; i<COCHES; i++) {
        switch (pid_hijo=fork())
        {
        case 0:
            // Si i es par entonces se crea un coche norte
            if (i % 2 == 0) {
                execl("./exec/coche", "coche", "N", PUENTE, MUTEXN, COCHESN, NULL);
            } else {
                execl("./exec/coche", "coche", "S", PUENTE, MUTEXS, COCHESS, NULL);
            }
            break;
        }
        sleep(rand() % MAX_T_LANZAR);
    }
    
    // Espera a que acaben todos sus hijos
    for ( i = 0; i < COCHES; i++)
    {
        waitpid(pids[i], 0, 0);
    }
    // Libera los recursos y acaba bien
    liberarRecursos();
    return EXIT_SUCCESS;
}
/**
 * @brief Metodo para liberar recursos
 * 
 * Este metodo libera todos los recursos utilizados por este programa.
 */
void liberarRecursos () {
    destruir_sem(PUENTE);
    destruir_var(COCHESN); destruir_var(COCHESS);
}
/**
 * @brief Metodo para finalizar procesos
 * 
 * Este metodo finaliza todos los procesos hijo que esten en ejecucion.
 */
void finalizarProcesos () {
    int i; printf ("\n--- Finalizando procesos ---\n");
    for (i = 0; i<COCHES; i++) {
        if (pids[i]) {
            printf("Finalizando proceso [%d]", pids[i]);
            kill (pids[i], SIGINT); printf("<OK>\n");
        }
    }
}
/**
 * @brief Metodo controlador
 * 
 * Este metodo libera y finaliza los recursos y procesos hijo cuando salte una señal de interrupcion
 */
void controlador (int senial) {
    finalizarProcesos(); liberarRecursos();
    printf("\nFin del programa (Ctrl + C)\n"); exit(EXIT_SUCCESS);
}