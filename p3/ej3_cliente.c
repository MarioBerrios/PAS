/*
Cliente que envia por una cola abierta para escritura una cadena de caracteres
recogida por teclado, mientras que el valor de esa cadena sea distinto a la palabra exit
*/

#include "ej3_common.h"
#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>     /* Manejo de señales */

// Prototipo de funcion
void funcionLog(char *);
void my_callback(int);
// Apuntador al fichero de log. No se usa en este ejemplo, pero le servira en ejercicio resumen
FILE *fLog = NULL;
// Cola del servidor
mqd_t mq_server;
mqd_t mq_client;

int main(int argc, char **argv) {

    if (signal(SIGINT, my_callback) == SIG_ERR)
        printf("No puedo asociar la señal SIGINT al manejador!\n");
    if (signal(SIGTERM, my_callback) == SIG_ERR)
        printf("No puedo asociar la señal SIGTERM al manejador!\n");

    // Buffer para intercambiar mensajes
    char buffer_server[MAX_SIZE];
    char buffer_client[MAX_SIZE];
    char buffer_aux [MAX_SIZE];
    // Nombre para la cola
    char serverQueue[100];
    char clientQueue[100];

    // Abrir la cola del servidor. La cola CLIENT_QUEUE le servira en ejercicio resumen.
    // No es necesario crearla si se lanza primero el servidor, sino el programa no funciona.

    // Nombre para la cola. Al concatenar el login sera unica en un sistema compartido.
    sprintf(serverQueue, "%s-%s", SERVER_QUEUE, getenv("USER"));
    printf("[Cliente]: El nombre de la cola del servidor es: %s\n", serverQueue);
    sprintf(clientQueue, "%s-%s", CLIENT_QUEUE, getenv("USER"));
    printf("[Cliente]: El nombre de la cola del cliente es: %s\n", clientQueue);

    mq_server = mq_open(serverQueue, O_WRONLY);
    if (mq_server == (mqd_t)-1) {
        perror("Error al abrir la cola del servidor");
        funcionLog("Error al abrir la cola del servidor");
        exit(-1);
    }
    printf("[Cliente]: El descriptor de la cola del servidor es: %d\n", (int)mq_server);

    mq_client = mq_open(clientQueue, O_RDONLY);
    if (mq_client == (mqd_t)-1) {
        perror("Error al abrir la cola del cliente");
        funcionLog("Error al abrir la cola del cliente");
        exit(-1);
    }
    printf("[Cliente]: El descriptor de la cola del cliente es: %d\n", (int)mq_client);

    printf("Mandando mensajes al servidor (escribir \"%s\" para parar):\n", MSG_STOP);
    do {
        printf("> ");

        /* Leer por teclado. Según la documentación, fgets lo hace de esta manera:
        It stops when either (n-1) characters are read, the newline character is read,
        or the end-of-file is reached, whichever comes first.
        Automáticamente fgets inserta el fin de cadena '\0'
        */
        fgets(buffer_server, MAX_SIZE, stdin);

        // Enviar y comprobar si el mensaje se manda
        if (mq_send(mq_server, buffer_server, MAX_SIZE, 0) != 0) {
            perror("Error al enviar el mensaje");
            funcionLog("Error al enviar el mensaje");
            exit(-1);
        }
        sprintf(buffer_aux, "Mensaje enviado: %s", buffer_server);
        funcionLog(buffer_aux);

        if (strncmp(buffer_server, MSG_STOP, strlen(MSG_STOP)) != 0){
            // Número de bytes leidos
            size_t bytes_read;
            // Recibir el mensaje
            bytes_read = mq_receive(mq_client, buffer_client, MAX_SIZE, NULL);
            if (bytes_read < 0) {
                perror("Error al recibir el mensaje");
                funcionLog("Error al recibir el mensaje");
                exit(-1);
            }
            //buffer_client[bytes_read] = '\0';
            printf("Recibido el mensaje: %s", buffer_client);
            sprintf(buffer_aux, "Mensaje recibido: %s", buffer_client);
            funcionLog(buffer_aux);
        }
        // Iterar hasta escribir el código de salida
    } while (strncmp(buffer_server, MSG_STOP, strlen(MSG_STOP)));

    funcionLog("Cerrando colas");
    // Cerrar la cola del servidor
    if (mq_close(mq_server) == (mqd_t)-1) {
        perror("Error al cerrar la cola del servidor");
        funcionLog("Error al cerrar la cola del servidor");
        exit(-1);
    }
    if (mq_close(mq_client) == (mqd_t)-1) {
        perror("Error al cerrar la cola del cliente");
        funcionLog("Error al cerrar la cola del cliente");
        exit(-1);
    }
    return 0;
}

/* Función auxiliar, escritura de un log.
No se usa en este ejemplo, pero le puede servir para algun
ejercicio resumen */
void funcionLog(char *mensaje) {
    int resultado;
    char nombreFichero[100];
    char mensajeAEscribir[300];
    time_t t;

    // Abrir el fichero
    sprintf(nombreFichero, "log-cliente.txt");
    if (fLog == NULL) {
        fLog = fopen(nombreFichero, "at");
        if (fLog == NULL) {
            perror("Error abriendo el fichero de log");
            exit(1);
        }
    }

    // Obtener la hora actual
    t = time(NULL);
    struct tm *p = localtime(&t);
    strftime(mensajeAEscribir, 1000, "[%Y-%m-%d, %H:%M:%S]", p);

    // Vamos a incluir la hora y el mensaje que nos pasan
    sprintf(mensajeAEscribir, "%s ==> %s\n", mensajeAEscribir, mensaje);

    // Escribir finalmente en el fichero
    resultado = fputs(mensajeAEscribir, fLog);
    if (resultado < 0)
        perror("Error escribiendo en el fichero de log");

    fclose(fLog);
    fLog = NULL;
}


void my_callback(int signal)
{
    printf("\nCapturé la señal %i\n", signal);
    printf("\nEnviando exit al servidor");

    char buffer_aux[MAX_SIZE];
    sprintf(buffer_aux, "Capturada la señal %i, enviando exit al servidor", signal);
    funcionLog(buffer_aux);

    if (mq_send(mq_server, "exit", MAX_SIZE, 0) != 0) {
        perror("Error al enviar el mensaje");
        funcionLog("Error al enviar el mensaje");
        exit(-1);
    }

    funcionLog("Cerrando colas");
    // Cerrar la cola del servidor
    if (mq_close(mq_server) == (mqd_t)-1) {
        perror("Error al cerrar la cola del servidor");
        funcionLog("Error al cerrar la cola del servidor");
        exit(-1);
    }
    if (mq_close(mq_client) == (mqd_t)-1) {
        perror("Error al cerrar la cola del cliente");
        funcionLog("Error al cerrar la cola del cliente");
        exit(-1);
    }
    exit(0);
}
