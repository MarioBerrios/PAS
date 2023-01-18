#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h> //Control de errores
#include <string.h> //Para la funcion strerror(), que permite describir el valor de errno como cadena.

int main() {
	// Para realizar el fork
	pid_t rf;
	int flag, status;
	// Para controlar los valores devueltos por las funciones (control de errores)
	int resultado;
	// Lo que vamos a leer y escribir de la tubería
	float suma;
	// Descriptores de los dos extremos de la tubería
	int fileDes[2];
	// Iterador
	int i=0;

	// Creamos la tubería
	resultado = pipe(fileDes);
	if(resultado==-1) {
		printf("\nERROR al crear la tubería.\n");
		exit(1);
	}
	
	rf = fork();
	switch (rf) {
		case -1:
			printf ("No se ha podido crear el proceso hijo...\n");
			exit(EXIT_FAILURE);
		case 0:
			printf ("[HIJO]: Mi PID es %d y mi PPID es %d\n", getpid(), getppid());
			
            //Cerramos la escritura de la cola
			if (close(fileDes[1]) == -1) {
                perror("Error en close"); 
                exit(EXIT_FAILURE);
            }
            //Recibimos un mensaje a través de la cola
            resultado = read( fileDes[0], &suma, sizeof(float));
            if(resultado != sizeof(float)) {
                printf("\n[HIJO]: ERROR al leer de la tubería...\n");
                exit(EXIT_FAILURE);
            }
            //En caso de no saber el tamaño del mensaje se debe controlar
            //los valores -1 (error en recibir), 0 (no se ha recibido nada -> tuberia cerrada)
            //y lo demás. Si es un string lo que se recibe usar strtok()

            // Imprimimos el campo que viene en la tubería
            printf("[HIJO]: Leo la suma %f de la tubería.\n", suma);
            // Cerrar el extremo que he usado

			if (close(fileDes[0]) == -1) { // Ya no vamos a usar la lectura más
                perror("Error en close"); 
                exit(EXIT_FAILURE);
            }
			printf("[HIJO]: Tubería cerrada ...\n");

			break;

		default:
			printf ("[PADRE]: Mi PID es %d y el PID de mi hijo es %d \n", getpid(), rf);
			
			if (close(fileDes[0]) == -1) { // No vamos a usar la escritura
                perror("Error en close"); 
                exit(EXIT_FAILURE);
            }
            
            srand(time(NULL));

            //Dos números entre 1.0 y 10.0
            float numeroAleatorio = (((float) rand()) / ((float) RAND_MAX)) * 9 + 1;
            float numeroAleatorio2 = (((float) rand()) / ((float) RAND_MAX)) * 9 + 1;
			printf("[PADRE]: Número aleatorio %f\n", numeroAleatorio);
			printf("[PADRE]: Número aleatorio %f\n", numeroAleatorio2);

            suma = numeroAleatorio + numeroAleatorio2;
			printf("[PADRE]: Escribo la suma %f en la tubería...\n", suma);
            
            resultado = write(fileDes[1], &suma, sizeof(float));
            if(resultado != sizeof(float)){
                printf("\n[PADRE]: ERROR al escribir en la tubería...\n");
                exit(EXIT_FAILURE);
            }

            // Cerrar el extremo que he usado
			if (close(fileDes[1]) == -1) {
                perror("Error en close"); 
                exit(EXIT_FAILURE);
            }
			printf("[PADRE]: Tubería cerrada...\n");

            /*Espera del padre a los hijos*/
	        while ( (flag=wait(&status)) > 0 ) 
	        {
		        if (WIFEXITED(status)) {
			        printf("Proceso Padre, Hijo con PID %ld finalizado, status = %d\n", (long int)flag, WEXITSTATUS(status));
		        } 
		        else if (WIFSIGNALED(status)) {  //Para seniales como las de finalizar o matar
			        printf("Proceso Padre, Hijo con PID %ld finalizado al recibir la señal %d\n", (long int)flag, WTERMSIG(status));
		        } 		
	        }
	        if (flag==(pid_t)-1 && errno==ECHILD)
	        {
		        printf("Proceso Padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
	        }
	        else
	        {
		        printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
		        exit(EXIT_FAILURE);
	        }			 
	}
	exit(EXIT_SUCCESS);
}