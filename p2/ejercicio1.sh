#!/bin/bash

for user in $(ps aux | sed -rne '1!s/^([^ ]+).*/\1/p' | sort | uniq); do
    SUMA=0
    for cpu in $(ps aux | sed -rne '1!s/^'$user'[ ]+[^ ]+[ ]+([^ ]+).*/\1/p' | sort | uniq); do
        SUMA=$(echo "$SUMA + $cpu" | bc)
    done 
    echo "$user está haciendo un uso de cpu del $SUMA"
done