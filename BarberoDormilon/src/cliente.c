/**
 * @file cliente.c
 * @brief Implementacion de un proceso cliente.
 * @author Daniel Diaz Garcia
 * @date 2025-03-22
 */
#include <semaforoI.h>
#include <memoriaI.h>
#include <definitions.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * @brief Funcion principal del proceso cliente.
 * 
 * @param argc Cantidad de argumentos.
 * @param argv Argumentos.
 * @return EXIT_SUCCESS si todo salio bien.
 */
int main (int argc, char *argv[]) {
    sem_t *mutex_wr, *wup_barber, *wup_client, *mutex_sit;
    int wr, num_clients;
    
    pid_t pid = getpid();

    srand(pid);

    mutex_wr=get_sem(MUTEX_WR); wup_barber=get_sem(WUP_BARBER); wup_client=get_sem(WUP_CLIENT); mutex_sit=get_sem(MUTEX_SIT);
    wr = obtener_var(WAITING_ROOM);

    printf("Cliente [%d] creado con exito.\n",pid);

    printf("Cliente [%d] llegando a la barberia ...\n",pid);
    sleep(rand() % 15);

    wait_sem(mutex_wr);

    consultar_var(wr, &num_clients);
    printf("Cliente [%d] viendo si la barberia esta llena ...\n",pid);
    if (num_clients == MAX_PPLE_WR) {
        printf("Cliente [%d] abandonando. Barberia llena.\n",pid);
        signal_sem(mutex_wr);
        return EXIT_SUCCESS;
    }

    modificar_var(wr, ++num_clients);
    printf("Cliente [%d] entrando en la barberia. Gente en la barberia: %d\n", pid, num_clients);

    signal_sem(mutex_wr);

    printf("Cliente [%d] esperado a cortarse el pelo.\n",pid);

    wait_sem(mutex_sit);
    
    printf("Cliente [%d] sentadose en el sillon.\n", pid);

    wait_sem(mutex_wr);
    
    consultar_var(wr, &num_clients);
    modificar_var(wr, --num_clients);

    printf("Gente en la barberia: %d\n",num_clients);
    signal_sem(mutex_wr);

    printf("Cliente [%d] despertando al Barbero ...\n",pid);
    signal_sem(wup_barber);
    wait_sem(wup_client);
    
    signal_sem(mutex_sit);
    printf("Cliente [%d] abandonando barberia.\n", pid);

    return EXIT_SUCCESS;
}