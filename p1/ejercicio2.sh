#!/bin/bash

if [ $# != 1 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "ejercicio2.sh <ABS_PATH>"
    exit
fi

#Este ABS_PATH tiene el nombre de la carpeta
ABS_PATH=$1

PER_DIR=750
PER_SH=u+x
PER_KEY=go-rwx
echo ""

echo "Cambiando permisos a los directorios"
#Con $find [PATH] -type podemos buscar directorios
for dir in $(find $ABS_PATH -type d); do
    echo $dir
    chmod $PER_DIR $dir
done
echo ""

echo "Cambiando permisos a los ficheros \".sh\""
#Con $find [PATH] -name <REG_EXP> podemos buscar por nombre según la expresión regular
for sh in $(find $ABS_PATH -name "*.sh"); do
    echo $sh
    chmod $PER_SH $sh
done
echo ""

echo "Cambiando permisos a los ficheros \".key\""
for key in $(find $ABS_PATH -name "*.key"); do
    echo $key
    chmod $PER_KEY $key
done
echo ""