/**
 * @file manager.c
 * @brief Implementacion de un proceso manager.
 * @author Daniel Diaz Garcia
 * @date 2025-03-22
 */
#include <errno.h>
#include <linux/limits.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <semaforoI.h>
#include <memoriaI.h>
#include <definitions.h>
#include <stdlib.h>
/**
 * @brief Funcion que instala un manejador de señal.
 */
void instalar_manejador_senhal ();
/**
 * @brief Funcion manejadora de señales.
 */
void manejador_senhal (int senhal);
/**
 * @brief Funcion que inicia la tabla de procesos hijos.
 */
void iniciar_tabla_procesos (int n_processes_client);
/**
 * @brief Funcion que crea los procesos hijos.
 */
void crear_procesos (int numClients);
/**
 * @brief Funcion que lanza un proceso cliente.
 */
void lanzar_proceso_cliente (const int table_index);
/**
 * @brief Funcion que lanza un proceso barbero.
 */
void lanzar_proceso_barbero ();
/**
 * @brief Funcion que termina los procesos hijos.
 */
void terminar_procesos (void);
/**
 * @brief Funcion que libera los recursos.
 */
void liberar_recursos ();
/**
 * @brief Tabla de procesos clientes y barbero.
 */
struct TProcess_t *g_process_table_clients, *g_process_table_barber;

/**
 * @brief Funcion principal del proceso manager.
 * 
 *  @param argc Cantidad de argumentos.
 *  @param argv Argumentos.
 *  @return EXIT_SUCCESS si todo salio bien.
 */
int main (int argc, char *argv[]) {
    crear_sem(WUP_BARBER, 0); crear_sem(WUP_CLIENT, 0);
    crear_sem(MUTEX_SIT, 1);crear_sem(MUTEX_WR, 1);
    crear_var(WAITING_ROOM, 0);

    instalar_manejador_senhal();
    iniciar_tabla_procesos(NUM_CLIENTS);
    crear_procesos(NUM_CLIENTS);

    for (int i = 0; i < NUM_CLIENTS; i++) {
        waitpid(g_process_table_clients[i].pid, 0, 0);
    }

    printf("\n[MANAGER] Terminación de programa (todos los procesos terminados).\n");
    if (kill(g_process_table_barber[0].pid,SIGINT) == -1) {
        fprintf(stderr, "[MANAGER] Error al usar kill() en proceso %d: %s.\n", g_process_table_barber[0].pid,strerror(errno));
    }
    liberar_recursos();

    return EXIT_SUCCESS;
}
/**
 * @brief Funcion que instala un manejador de señal.
 * 
 * La funcion instala un manejador de señal para el proceso manager.
 */
void instalar_manejador_senhal () {
    if (signal(SIGINT, manejador_senhal ) == SIG_ERR) {
        fprintf(stderr, "[MANAGER] Error al instalar el manejador de señal: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
/**
 * @brief Funcion manejadora de señales.
 * 
 * La funcion maneja la señal SIGINT (Ctrl+C) y termina el programa.
 * 
 * @param sign Señal a manejar.
 */
void manejador_senhal (int sign) {
    printf("\n[MANAGER] Terminación del programa (Ctrl+C).\n");
    terminar_procesos();
    liberar_recursos();
    exit(EXIT_SUCCESS);
}
/**
 * @brief Funcion que libera los recursos.
 * 
 * La funcion libera los recursos utilizados por el programa.
 */
void liberar_recursos () {
    destruir_sem(WUP_BARBER); destruir_sem(WUP_CLIENT); destruir_sem(MUTEX_SIT); destruir_sem(MUTEX_WR);
    destruir_var(WAITING_ROOM);
}
/**
 * @brief Funcion que termina los procesos hijos.
 * 
 * La funcion termina los procesos hijos que se encuentran en
 * la tabla de procesos.
 */
void terminar_procesos (void) {
    int i;

    printf("\n----- [MANAGER] Terminar con los procesos hijos ejecutándose -----\n");
    for (i = 0; i < NUM_CLIENTS; i++) {
        if (g_process_table_clients[i].pid != 0)
        {
            printf("[MANAGER] Terminar con proceso %s [%d] ...\n", g_process_table_clients[i].clase, g_process_table_clients[i].pid);
            if (kill(g_process_table_clients[i].pid, SIGINT) == -1) {
                fprintf(stderr, "[MANAGER] Error al usar kill() en proceso %d: %s.\n", g_process_table_clients[i].pid,strerror(errno));
            }
        }
    }

    printf("[MANAGER] Terminar con proceso %s [%d] ...", g_process_table_barber[0].clase, g_process_table_barber[0].pid);
    if (kill(g_process_table_barber[0].pid, SIGINT) == -1) {
        fprintf(stderr, "[MANAGER] Error al usar kill() en proceso %d: %s.\n", g_process_table_barber[0].pid,strerror(errno));
    }
}
/**
 * @brief Funcion que inicia la tabla de procesos hijos.
 * 
 * La funcion inicia la tabla de procesos hijos con el numero de procesos
 * clientes que se van a crear.
 * 
 * @param n_processes_client Numero de procesos clientes a crear.
 */
void iniciar_tabla_procesos (int n_processes_client) {
    g_process_table_clients = malloc(n_processes_client*sizeof(struct TProcess_t));
    g_process_table_barber = malloc(sizeof(struct TProcess_t));
    
    for (int i = 0; i < n_processes_client; i++) {
        g_process_table_clients[i].pid = 0;
    }

    g_process_table_barber[0].pid = 0;
}
/**
 * @brief Funcion que crea los procesos hijos.
 * 
 * La funcion crea los procesos hijos, primero se crea el proceso barbero
 * y despues se crean los procesos clientes.
 * 
 * @param numClients Numero de procesos clientes a crear.
 */
void crear_procesos (int numClients) {
    int index_table = 0;
    lanzar_proceso_barbero(index_table);
    for (int i = 0; i <= numClients; i++) {
        lanzar_proceso_cliente(index_table);
        index_table++;
    }
}
/**
 * @brief Funcion que lanza un proceso barbero.
 * 
 * La funcion lanza un proceso barbero y lo guarda en la tabla de procesos.
 * 
 * @param index_table Indice de la tabla de procesos.
 */
void lanzar_proceso_barbero (int index_table) {
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "[MANAGER] Error al lanzar proceso %s: %s.\n", CLASS_BARBER, strerror(errno));
        break;
    case 0:
        
        if (execl(BARBER_PATH, CLASS_BARBER, NULL) == -1) {
            fprintf(stderr, "[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASS_BARBER, strerror(errno));
        }
        exit(EXIT_FAILURE);
        break;
    default:
        g_process_table_barber[index_table].pid = pid;
        g_process_table_barber[index_table].clase = CLASS_BARBER;
        break;
    }
}
/**
 * @brief Funcion que lanza un proceso cliente.
 * 
 * La funcion lanza un proceso cliente y lo guarda en la tabla de procesos.
 * 
 * @param index_table Indice de la tabla de procesos.
 */
void lanzar_proceso_cliente (int index_table) {
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "[MANAGER] Error al lanzar proceso %s: %s.\n", CLASS_CLIENT, strerror(errno));
        break;
    case 0:
        if (execl(CLIENT_PATH, CLASS_CLIENT, NULL) == -1) {
            fprintf(stderr, "[MANAGER] Error usando execl() en el proceso %s: %s.\n", CLASS_CLIENT, strerror(errno));
        }
        exit(EXIT_FAILURE);
        break;
    default:
        g_process_table_clients[index_table].pid = pid;
        g_process_table_clients[index_table].clase = CLASS_CLIENT;
        break;
    }
}