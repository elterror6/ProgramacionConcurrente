/**
 * @file coche.c
 * @brief Programa Coche
 * 
 * Este programa representa un coche que desea cruzar por un puente para representar el problema
 * del puente de un solo carril.
 * 
 * @author Daniel Diaz Garcia
 * @date 2025-03-10
 */
#include <memoriaI.h>
#include <semaforoI.h>


/**
 * @def MAX_TIME_CROSS
 * @brief Maximo tiempo de cruce
 * 
 * Indica el tiempo maximo que va a tardar un coche en cruzar el puente.
 */
#define MAX_TIME_CROSS 5

void coche(); void cruzar(); // Declaracion de metodos

/**
 * @brief Metodo main
 * 
 * Metodo principal del programa coche.c.
 * 
 * @param argc Cantidad de argumentos.
 * @param argv Array de cadenas con los argumentos.
 *             'argv[0]' es el nombre del programa.
 *             'argv[1]' indica por donde sale el coche (N o S).
 *             'argv[2]' es el id del semaforo puente.
 *             'argv[3]' es el id del semaforo de uno de los lados del puente.
 *             'argv[4]' es el id de la variable de memoria compartida que contabiliza los coches de un lado del puente.
 * @return 0 si la ejecucion fue exitosa.
 */
int main (int argc, char *argv[]) {
    coche(argv[1], argv[2], argv[3], argv[4]);

    return 0;
}
/**
 * @brief Metodo coche
 * 
 * Este metodo recibe una salida, que representa por cual lado del puente va a cruzar, el id de un semaforo puente, el id del
 * semaforo de ese lado del puente, es decir, semaforo norte o sur, y la variable de memoria compartida que contiene el numero
 * de coches que hay al norte o al sur.
 * 
 * @param exit Puntero a una variable de tipo caracter que representa de donde viene un coche, del norte ("N") o del sur ("S").
 * @param id_brigde Puntero a una variable de tipo caracter que representa el id del semaforo puente.
 * @param id_mutex Puntero a una variable de tipo caracter que representa el id de un semaforo de alguno de los dos extremos del puente.
 * @param id_num_coches Puntero a una variable de tipo caracter que representa el id de la variable compartida que contiene el numero de coches que hay por un lado del puente.
 */
void coche (char *exit, char *id_brigde, char *id_mutex, char *id_num_coches) {
    int num_coches_handle, valor;
    sem_t *puente, *mutex;

    srand( (int) getpid() );

    puente = get_sem(id_brigde);
    mutex = get_sem(id_mutex);
    num_coches_handle = obtener_var(id_num_coches);

    // SECCION CRITICA
    wait_sem(mutex);

    // Meto coche en el puente
    consultar_var( num_coches_handle,&valor );
    modificar_var( num_coches_handle, ++valor );

    if ( valor == 1 ) {
        wait_sem(puente); // Espera acceso al puente
    }
    signal_sem(mutex);

    cruzar(exit); // Cruza y tarda en cruzar un tiempo aleatorio

    wait_sem(mutex);

    // Saco coche del puente
    consultar_var( num_coches_handle, &valor);
    modificar_var( num_coches_handle, --valor);

    if ( valor == 0 ) {
        signal_sem(puente); // Libera el puente
    }

    signal_sem(mutex);

}
/**
 * @brief Metodo cruzar
 * 
 * Este metodo indica por salida por pantalla que el coche esta cruzando por el puente de un
 * lado al otro del mismo.
 * 
 * @param exit Puntero a una variable de tipo caracter que representa de donde viene un coche, del norte ("N") o del sur ("S").
 */
void cruzar (char *exit) {
    // Imprime de donde cruza hacia donde
    if (strcmp(exit, "N") == 0) {
        printf("%d cruzando de N a S ...\n", getpid());
    } else {
        printf("%d cruzando S a N ...\n", getpid());
    }
    // Simula que cruza
    sleep( rand() %MAX_TIME_CROSS +1 );
}