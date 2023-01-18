#!/bin/bash

if [ $# != 2 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "./ejercicio3.sh <directorio_origen> <directorio_destino>"
    exit
fi

DIR_ORIG=$1
if [ ! -d $DIR_ORIG ]; then
    echo "El directorio de origen no existe"
    exit
fi
NAME_FILE=$(basename $DIR_ORIG)

DIR_DEST=$2
if [ ! -d $DIR_DEST ]; then
    mkdir $DIR_DEST
fi

#El comando $date [+FORMAT] te da la fecha en un formato concreto
FECHA=$(date +%Y%m%d)
TAR_FILE=${NAME_FILE}_$FECHA.tar.gz
if [ -f $DIR_DEST/$TAR_FILE ]; then
    echo "La copia de seguridad ya se ha realizado hoy"
    exit
fi

#La barra se necesitará dependiendo de la entrada
tar -czf $DIR_DEST/$TAR_FILE $DIR_ORIG