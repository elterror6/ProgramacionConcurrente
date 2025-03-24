/**
 * @file barbero.c
 * @brief Implementacion de un proceso barbero.
 * @author Daniel Diaz Garcia
 * @date 2025-03-22
 */
#include <semaforoI.h>
#include <memoriaI.h>
#include <definitions.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * @brief Funcion que simula el corte de pelo.
 */
void cortar ();
/**
 * @brief Funcion principal del proceso barbero.
 * 
 * @param argc Cantidad de argumentos.
 * @param argv Argumentos.
 * @return EXIT_SUCCESS si todo salio bien.
 */
int main (int argc, char *argv[]) {
    sem_t *wup_barber, *wup_client;
    pid_t pid = getpid();

    wup_barber = get_sem(WUP_BARBER);
    wup_client = get_sem(WUP_CLIENT);

    printf("Barbero [%d] preparado.\n", pid);

    while (1) {

        printf("Barbero [%d] durmiendo (zzz)...\n",pid);
        wait_sem(wup_barber);
        printf("Barbero [%d] despierto.\n",pid);

        cortar();

        printf("Barbero [%d] ya termino de cortar el pelo.\n",pid);
        signal_sem(wup_client);
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Funcion que simula el corte de pelo.
 * 
 * La funcion simula el corte de pelo, el barbero tarda un tiempo aleatorio en cortar el pelo.
 */
void cortar () {
    pid_t pid = getpid();
    printf("Barbero [%d] cortando el pelo.\n", pid);
    sleep(rand() % 3);
}