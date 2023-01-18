#!/bin/bash

if [ $# != 1 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "./ejercicio4.sh <directorio/fichero>"
    exit
fi

#No utilizar el nombre PATH para ninguna variable.
#Es nombre reservado
ABS_PATH=$1
CONTADOR=1

for x in $(find $ABS_PATH -type f); do
    NAME=$(basename $x)
    #También se podría
    CHARACTERS=${#NAME}
    #let CHARACTERS=$(echo "$NAME" | wc -m)-1
    #La flag -e permite escape de caracteres especiales
    echo -e "$CONTADOR\t$NAME\t$CHARACTERS"
    let CONTADOR=$CONTADOR+1
done