/**
 * @file definitions.h
 * @brief Definiciones para el proyecto de barbero dormilon.
 * 
 * Contiene las definiciones de semaforos y de memoria compartida.
 * 
 * @author Daniel Diaz Garcia
 * @date 2025-03-22
 */

/**
 * @def WUP_BARBER
 * @brief Define el nombre del semaforo que levanta al barbero.
 */
#define WUP_BARBER "wupbaber"
/**
 * @def WUP_CLIENT
 * @brief Define el nombre del semaforo que levanta al cliente.
 */
#define WUP_CLIENT "wupclient"
/**
 * @def MUTEX_WR
 * @brief Define el nombre del semaforo mutex que da acceso a la variable de memoria compartida de la sala de espera.
 */
#define MUTEX_WR "mutexwr"
/**
 * @def MUTEX_SIT
 * @brief Define el nombre del semaforo mutex que representa el sillon del barbero.
 */
#define MUTEX_SIT "mutexs"

/**
 * @def WAITING_ROOM
 * @brief Define el nombre de la variable de memoria compartida de la sala de espera.
 */
#define WAITING_ROOM "waitingroom"

/**
 * @def MAX_PPLE_WR
 * @brief Define el numero maximo de personas que 
 */
#define MAX_PPLE_WR 5

/**
 * @def CLASS_BARBER
 * @brief Define una cadena de caracteres para representar el proceso barbero.
 */
#define CLASS_BARBER "BARBER"
/**
 * @def CLASS_CLIENT
 * @brief Define una cadena de caracteres para representar el proceso cliente.
 */
#define CLASS_CLIENT "CLIENT"

/**
 * @def NUM_CLIENTS
 * @brief Define el numero de procesos cliente a crear.
 */
#define NUM_CLIENTS 10

/**
 * @def BARBER_PATH
 * @brief Define la ruta del binario del barbero.
 */
#define BARBER_PATH "./exec/barbero"
/**
 * @def CLIENT_PATH
 * @brief Define la ruta del binario del cliente.
 */
#define CLIENT_PATH "./exec/cliente"

/**
 * @struct TProxess_t
 * @brief Representa una tabla de procesos.
 */
struct TProcess_t
{
    pid_t pid; /**< PID del proceso. */
    char *clase; /**< Cadena de carácteres que representa un proceso. */  
};
