#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>

void helpMenu();
void imprimirUsuario(char *user, bool mflag);
void imprimirGrupo(char *group);
void imprimirTodosGrupos();

int main(int argc, char **argv) {
    int c;
    struct group *gr;

    static struct option long_options[] = {
        {"user", required_argument, NULL, 'u'},
        {"group", required_argument, NULL, 'g'},
        {"active", no_argument, NULL, 'a'},
        {"maingroup", no_argument, NULL, 'm'},
        {"allgroups", no_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    char *uvalue = NULL;
    char *gvalue = NULL;
    bool aflag = false;
    bool mflag = false;
    bool sflag = false;
    bool hflag = false;


    /* Deshabilitar la impresión de errores por defecto */
    opterr=0;
    while ((c = getopt_long(argc, argv, "u:g:amsh", long_options, NULL)) != -1) {
        //printf("optind: %d, optarg: %s, optopt: %c\n", optind, optarg, optopt);

        switch (c) {
            case 'u':
                // printf("Opción -u con valor '%s'\n", optarg);
                uvalue = optarg;
                break;

            case 'g':
                gvalue = optarg;
                break;

            case 'a':
                // printf("Opción -a\n");
                aflag = true;
                break;

            case 'm':
                mflag = true;
                break;

            case 's':
                sflag = true;
                break;

            case 'h':
                hflag = true;
                break;

            case '?':
                // Opcion no reconocida o INCOMPLETA (sin argumento). Probar
                // tambien la diferencia entre ejecutar %$>./a.out m ó
                // %$>./a.out -m
                if (optopt == 'u' || optopt == 'g')     // Para el caso de que 'u' o 'g' no tenga el argumento
                                                        // obligatorio.
                    fprintf(stderr,
                            "La opción \"-%c\" requiere un argumento. Valor de opterr = "
                            "%d\n",
                            optopt, opterr);

                else if (isprint(optopt)) // Se mira si el caracter es imprimible
                    fprintf(stderr,
                            "Opción desconocida \"-%c\". Valor de opterr = %d\n",
                            optopt, opterr);

                else // Caracter no imprimible o especial
                    fprintf(stderr, "Caracter `\\x%x'. Valor de opterr = %d\n",
                            optopt, opterr);

                helpMenu();
                exit(1); // Finaliza el programa

            default:
                abort();
        }
    }

    /* Imprimir el resto de argumentos de la línea de comandos que no son
     * opciones con "-" */
    if (optind < argc) {
        /*
        printf("Argumentos ARGV que no son opciones: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        putchar('\n');
        */
        printf("Se han incluido argumentos no válidos\n");
        helpMenu();
        exit(1);
    }

    // Comprobación -a y -u no están a la vez
    if (aflag && uvalue != NULL){
        printf("La opción \"-a\" y \"-u\" no pueden ir juntos\n");
        helpMenu();
        exit(1);
    }

    // Comprobación -m va con -a o -u
    if (mflag && !(aflag || uvalue != NULL)){
        printf("La opción \"-m\" debe de ir acompañada de \"-a\" o \"-u\"\n");
        helpMenu();
        exit(1);
    }

    if (hflag){
        helpMenu();
        exit(0);
    }

    if (uvalue != NULL){
        imprimirUsuario(uvalue, mflag);
    }

    if (gvalue != NULL){
        imprimirGrupo(gvalue);
    }

    if (aflag){
        char *lgn;
        if ((lgn = getenv("USER")) == NULL){
            printf("Fallo al obtener información del usuario activo\n");
            exit(1);
        }
        imprimirUsuario(lgn, mflag);
    }

    if (sflag){
        imprimirTodosGrupos();
    }
}

void helpMenu(){
    printf("Uso del programa: ej1.c [opciones]\n");
    printf("Opciones:\n");
    printf("-h, --help\t\t\tImprimir ayuda\n");
    printf("-u, --user (<nombre>|<uid>)\tInformación sobre el usuario\n");
    printf("-a, --active\t\t\tInformación sobre el usuario activo actual\n");
    printf("-m, --maingroup\t\t\tAdemás de la información de usuario, imprime información del grupo principal\n");
    printf("-g, --group (<nombre>|<gid>)\tInformación sobre el grupo\n");
    printf("-a, --allgroups\t\t\tMuestra información de todos los grupos del sistema\n");
}

void imprimirUsuario(char *user, bool mflag){
    int uid;
    struct passwd *pw;
    if ((uid = atoi(user)) != 0){
        if ((pw = getpwuid(uid)) == NULL){
            fprintf(stderr, "Fallo al obtener información de usuario.\n");
            exit(1);
        }
    }
    else {
        if ((pw = getpwnam(user)) == NULL){
            fprintf(stderr, "Fallo al obtener información de usuario.\n");
            exit(1);
        }
    }

    printf("INFORMACIÓN DE USUARIO\n");
    printf("Nombre: %s\n", pw->pw_gecos);
    printf("Login: %s\n", pw->pw_name);
    printf("Password: %s\n", pw->pw_passwd);
    printf("UID: %d\n", pw->pw_uid);
    printf("Home: %s\n", pw->pw_dir);
    printf("Shell: %s\n", pw->pw_shell);
    printf("Número de grupo principal: %d\n", pw->pw_gid);
    printf("\n");

    if(mflag){
        char grupo [10];
        sprintf(grupo, "%d", pw->pw_gid);
        imprimirGrupo(grupo);
    }
}

void imprimirGrupo(char *group){
    int gid;
    struct group *gr;
    if ((gid = atoi(group)) != 0){
        if ((gr = getgrgid(gid)) == NULL){
            fprintf(stderr, "Fallo al obtener información de grupo.\n");
            exit(1);
        }
    }
    else {
        if ((gr = getgrnam(group)) == NULL){
            fprintf(stderr, "Fallo al obtener información de grupo.\n");
            exit(1);
        }
    }

    printf("INFORMACIÓN DE GRUPO\n");
    printf("Nombre: %s\n", gr->gr_name);
    printf("Password: %s\n", gr->gr_passwd);
    printf("GID: %d\n", gr->gr_gid);

    printf("Miembros secundarios:\n");
    int index = 0;
    while (gr->gr_mem[index] != NULL){
        printf("\t");
        printf(gr->gr_mem[index]);
        printf("\n");

        index++;
    }
    
    printf("\n");
}

void imprimirTodosGrupos(){
    FILE *f;
    char linea [100];

    if((f=fopen("/etc/group", "r"))==NULL){
        printf("\nError: error al abrir el fichero /etc/group");
        exit(-1);
    }

    while((fgets(linea, 100, f))!= NULL){
        char *token = strtok(linea, ":");
        //printf("token = %s \n", token);
        imprimirGrupo(token);
    }

    fclose(f);
}