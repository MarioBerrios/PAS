#!/bin/bash

#Con $# se sabe el número de argumentos que ha recibido el script sin contar el nombre
if [ $# != 3 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "ejercicio1.sh <ABS_PATH> <N_SUBDIR> <N_CHARS>"
    exit
fi

#El nombre de la carpeta va en ABS_PATH
#En caso de que solo sea el nombre en la ruta actual, usar pwd
#ABS_PATH=$(pwd)/$1

ABS_PATH=$1
N_SUBDIR=$2
N_CHARS=$3

if [ -d $ABS_PATH ]; then
    #Con $read -p podemos incluir un mensaje para el usuario
    read -p "La carpeta ya existe ¿desea borrarla? [y/n]: " elim
    if [ $elim == "y" ]; then
        #Con $rm -r borramos ABS_PATH de forma recursiva por ser un directorio
        rm -r $ABS_PATH
        mkdir $ABS_PATH
    else
        exit
    fi
else
    mkdir $ABS_PATH
fi

#El comando $seq N crea una secuencia de numeros desde 0 hasta N
for subdir in $(seq $N_SUBDIR); do
    SUBDIR=$(tr -dc A-Za-z0-9 < /dev/urandom | head -c $N_CHARS)
    mkdir $ABS_PATH/$SUBDIR

    #Si ponemos elementos los tomará como una lista
    for ext in .sh .html .key .txt; do
        FILE=$(tr -dc A-Za-z0-9 < /dev/urandom | head -c $N_CHARS)
        touch $ABS_PATH/$SUBDIR/$FILE$ext
    done
done