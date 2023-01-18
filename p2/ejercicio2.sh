#!/bin/bash

if [ $# != 3 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "ejercicio1.sh <FILE> <N_PINGS> <TIMEOUT>"
    exit
fi

FILE=$1
PINGS=$2
TIMEOUT=$3

# while read ip; do
#     salida=$(ping -c $PINGS -W $TIMEOUT $ip)
#     if [ $? == 0 ]; then
#         # Posible regex ^64 bytes from ([0-9\.]+): icmp_seq=[0-9]+ ttl=[0-9]+ time=([0-9,]+) si usamos la segunda linea
#         SALIDA_EXITO+=$(echo "$salida" | sed -rne 's/^round-trip min\/avg\/max\/stddev = [0-9,]+\/([0-9,]+)\/.*/'$ip' \1 ms/p')
#         SALIDA_EXITO+='\n'
#     else
#         SALIDA_FALLO+="La ip $ip no respondió en $TIMEOUT segundos.\n"
#     fi
# done < $FILE

for ip in $(cat $FILE); do
    salida=$(ping -c $PINGS -W $TIMEOUT $ip)
    if [ $? == 0 ]; then
        # Posible regex ^64 bytes from ([0-9\.]+): icmp_seq=[0-9]+ ttl=[0-9]+ time=([0-9,]+) si usamos la segunda linea
        SALIDA_EXITO+=$(echo "$salida" | sed -rne 's/^round-trip min\/avg\/max\/stddev = [0-9,]+\/([0-9,]+)\/.*/'$ip' \1 ms/p')
        SALIDA_EXITO+='\n'
    else
        SALIDA_FALLO+="La ip $ip no respondió en $TIMEOUT segundos.\n"
    fi
done

echo -e "$SALIDA_EXITO" | sort -nk 2,2
echo " "
echo -e "$SALIDA_FALLO"