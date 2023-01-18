#!/bin/bash

if [ $# != 2 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "./ejercicio5.sh <directorio> <num_horas>"
    exit
fi

ABS_PATH=$1
NUM_HORAS=$2

let SECONDS_HOURS=$NUM_HORAS*60*60

SECONDS_EPOCH=$(date +%s)
let SECONDS_EPOCH_HORUS=$SECONDS_EPOCH-$SECONDS_HOURS

for x in $(find $ABS_PATH -type f); do
    SECONDS_MOD=$(stat -c %Y $x)
    if [ $SECONDS_MOD -ge $SECONDS_EPOCH_HORUS ]; then
        echo "$x"
    fi
done