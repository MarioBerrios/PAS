/*
Servidor que lee de una cola abierta para lectura una cadena de caracteres y la
imprime por pantalla.

Lo hace mientras que el valor de esa cadena sea distinto a la palabra exit.
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

// Prototipo de funcionn
void funcionLog(char *);
// Apuntador al fichero de log.  No se usa en este ejemplo, pero le servira en ejercicio resumen
FILE *fLog = NULL;

int main(int argc, char **argv) {
    // Cola del servidor
    mqd_t mq_server;
    mqd_t mq_client;
    // Atributos de la cola
    struct mq_attr attr;
    // Buffer para intercambiar mensajes
    char buffer_server[MAX_SIZE];
    char buffer_client[MAX_SIZE];
    char buffer_aux[MAX_SIZE];
    // flag que indica cuando hay que parar. Se escribe palabra exit
    int must_stop = 0;
    // Inicializar los atributos de la cola
    attr.mq_maxmsg = 10;        // Maximo número de mensajes
    attr.mq_msgsize = MAX_SIZE; // Maximo tamaño de un mensaje
    // Nombre para la cola
    char serverQueue[100];
    char clientQueue[100];

    // Nombre para la cola. Al concatenar el login sera unica en un sistema compartido.
    sprintf(serverQueue, "%s-%s", SERVER_QUEUE, getenv("USER"));
    printf("[Servidor]: El nombre de la cola del servidor es: %s\n", serverQueue);

    sprintf(clientQueue, "%s-%s", CLIENT_QUEUE, getenv("USER"));
    printf("[Servidor]: El nombre de la cola del cliente es: %s\n", clientQueue);

    // Crear la cola de mensajes del servidor. La cola CLIENT_QUEUE le servira en ejercicio resumen
    mq_server = mq_open(serverQueue, O_CREAT | O_RDONLY, 0644, &attr);
    mq_client = mq_open(clientQueue, O_CREAT | O_WRONLY, 0644, &attr);
    // mq_server = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0644, &attr);

    if (mq_server == (mqd_t)-1) {
        perror("Error al abrir la cola del servidor");
        funcionLog("Error al abrir la cola del servidor");
        exit(-1);
    }
    printf("[Servidor]: El descriptor de la cola del servidor es: %d\n", (int)mq_server);

    if (mq_client == (mqd_t)-1) {
        perror("Error al abrir la cola del cliente");
        funcionLog("Error al abrir la cola del cliente");
        exit(-1);
    }
    printf("[Servidor]: El descriptor de la cola del cliente es: %d\n", (int)mq_client);

    do {
        // Número de bytes leidos
        ssize_t bytes_read;

        // Recibir el mensaje
        bytes_read = mq_receive(mq_server, buffer_server, MAX_SIZE, NULL);
        // Comprar que la recepción es correcta (bytes leidos no son negativos)
        if (bytes_read < 0) {
            perror("Error al recibir el mensaje");
            funcionLog("Error al recibir el mensaje");
            exit(-1);
        }
        // Cerrar la cadena
        //buffer[bytes_read] = '\0';

        // Comprobar el fin del bucle
        if (strncmp(buffer_server, MSG_STOP, strlen(MSG_STOP)) == 0){
            must_stop = 1;
            funcionLog("Recibido exit");
        }
        else{
            printf("Recibido el mensaje: %s", buffer_server);
            sprintf(buffer_aux, "Mensaje recibido: %s", buffer_server);
            funcionLog(buffer_aux);
            size_t size_message = strlen(buffer_server) - 1;

            sprintf(buffer_client, "Número de caracteres recibidos: %zu\n", size_message);
            printf("[CONTROL]: Enviando mensaje: %s", buffer_client);
            if (mq_send(mq_client, buffer_client, MAX_SIZE, 0) != 0) {
                perror("Error al enviar el mensaje");
                funcionLog("Error al enviar el mensaje");
                exit(-1);
            }
            sprintf(buffer_aux, "Mensaje enviado: %s", buffer_client);
            funcionLog(buffer_aux);
        }

    } while (!must_stop); // Iterar hasta que llegue el código de salida, es decir, la palabra exit

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

    funcionLog("Eliminando colas");
    // Eliminar la cola del servidor
    if (mq_unlink(serverQueue) == (mqd_t)-1) {
        perror("Error al eliminar la cola del servidor");
        funcionLog("Error al eliminar la cola del servidor");
        exit(-1);
    }
    if (mq_unlink(clientQueue) == (mqd_t)-1) {
        perror("Error al eliminar la cola del cliente");
        funcionLog("Error al eliminar la cola del cliente");
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
    sprintf(nombreFichero, "log-servidor.txt");
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
