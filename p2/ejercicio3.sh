#!/bin/bash

if [ $# != 1 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "ejercicio1.sh <FILE>"
    exit
fi

#OLDIFS=$IFS
#IFS=$'\n'
#IFS=$OLDIFS

FILE=$1
CONTADOR=1
#TEXTO=($(cat $FILE))
#echo "${TEXTO[*]}" | sort --sort=general-numeric

# Se le quita primero los puntos y comas y se pone cada palabra en
# una linea distinta. Luego se pasan mayus a minus, se quitan los espacios, se ordena y se eliminan repetidos.
for word in $(cat $FILE | tr '.|,' ' ' | tr ' ' '\n' | tr '[:upper:]' '[:lower:]' | grep -Ev '^\s*$' | sort | uniq); do
    CHARACTERS=${#word}
    echo -e "$CONTADOR\t$word $CHARACTERS"
    let CONTADOR=$CONTADOR+1
done

# for word in $(cat $FILE | sed -re 's/,|\.//g' | sed -re 's/\s+/\n/g' | sed -re 's/(.*)/\L\1/g' | sort | uniq); do
#     CHARACTERS=${#word}
#     echo -e "$CONTADOR\t$word $CHARACTERS"
#     let CONTADOR=$CONTADOR+1
# done