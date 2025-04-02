/**
 * @file definitions.h
 * @brief Definiciones y constantes utilizadas en el programa de los filósofos comensales.
 * 
 * Este archivo contiene las definiciones de los buzones, los filósofos y los tiempos de espera.
 * 
 * @author Daniel Díaz García
 * @date 2025-04-02
 */

 /**
  * @def BUZON_MESA
  * @brief Nombre del buzón de la mesa.
  */
#define BUZON_MESA "/buzon_mesa"
/**
 * @def BUZON_PALILLO
 * @brief Prefijo del nombre de los buzones de los palillos.
 */
#define BUZON_PALILLO "/buzon_palillo_"
/**
 * @def FILOSOFOS
 * @brief Número de filosofos que se van a ejecutar.
 */
#define FILOSOFOS 5


/**
 * @def MAX_TIME_PENSAR
 * @brief Tiempo máximo que un filósofo puede pensar.
 */
#define MAX_TIME_PENSAR 7
/**
 * @def MAX_TIME_COMER
 * @brief Tiempo máximo que un filósofo puede comer.
 */
#define MAX_TIME_COMER 5