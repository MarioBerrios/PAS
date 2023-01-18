#!/bin/bash

if [ $# != 1 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "ejercicio1.sh <N_BLOQUES>"
    exit
fi

cifras=$1

OLDIFS=$IFS
IFS=$'\n'

echo "1) Sistemas de ficheros cuyo número de bloques tenga al menos $cifras cifras"
for fila in $(df | sed -rne '1!p'); do
    bloques=$(echo "$fila" | sed -rne 's/^[^ ]+[ ]+([0-9]+)[ ]+.*$/\1/p')
    #echo $bloques
    if [ "${#bloques}" -ge "$cifras" ]; then
        echo "$fila"
    fi
done
echo ""
echo "2) Sistemas de ficheros cuyo porcentaje de uso sea inferior al 10%"
for fila in $(df | sed -rne '1!p'); do
    porcentaje=$(echo "$fila" | sed -rne 's/^[^ ]+[ ]+[0-9]+[ ]+[0-9]+[ ]+[0-9]+[ ]+([0-9]+)%[ ]+.*$/\1/p')
    if [ "$porcentaje" -lt "10" ]; then
        echo "$fila"
    fi
done
echo ""
echo "3) Sistemas de ficheros cuyo porcentaje de uso sea de al menos el 90%"
for fila in $(df | sed -rne '1!p'); do
    porcentaje=$(echo "$fila" | sed -rne 's/^[^ ]+[ ]+[0-9]+[ ]+[0-9]+[ ]+[0-9]+[ ]+([0-9]+)%[ ]+.*$/\1/p')
    if [ "$porcentaje" -ge "90" ]; then
        echo "$fila"
    fi
done

IFS=$OLDIFS

#echo "1) Sistemas de ficheros cuyo número de bloques tenga al menos $cifras cifras"
#df | while read fila; do
#    bloques=$(echo "$fila" | sed -rne 's/^[^ ]+[ ]+([0-9]+)[ ]+.*$/\1/p')
#    #echo $bloques
#    if [ "${#bloques}" -ge "$cifras" ]; then
#        echo "$fila"
#    fi
#done | column -t -s " "
#echo " "
#echo "2) Sistemas de ficheros cuyo porcentaje de uso sea inferior al 10%"
#df | sed -rne '1!p' | while read fila; do
#    porcentaje=$(echo "$fila" | sed -rne 's/^[^ ]+[ ]+[0-9]+[ ]+[0-9]+[ ]+[0-9]+[ ]+([0-9]+)%[ ]+.*$/\1/p')
#    if [ "$porcentaje" -lt "10" ]; then
#        echo "$fila"
#    fi
#done | column -t -s " "