#!/bin/bash

file=/etc/passwd
# Estructura
# nombre:password:uid:gid:comentario:home:shell
echo "1) Usuarios del grupo 46"
for user in $(cat $file);do
    if [ "$(echo "$user" | sed -rne 's/^[^:]+:[^:]+:[0-9]+:([0-9]+).*/\1/p')" == "46" ]; then
        echo $user
    fi
done
echo " "
echo "2) Usuarios de los grupos 33, 34 o 38"
for user in $(cat $file);do
    if [ "$(echo "$user" | sed -rne 's/^[^:]+:[^:]+:[0-9]+:([0-9]+).*/\1/p')" == "33" ] || 
    [ "$(echo "$user" | sed -rne 's/^[^:]+:[^:]+:[0-9]+:([0-9]+).*/\1/p')" == "34" ] || 
    [ "$(echo "$user" | sed -rne 's/^[^:]+:[^:]+:[0-9]+:([0-9]+).*/\1/p')" == "38" ]; then
        echo $user
    fi
done
echo " "
echo "3) Usuarios con UID de 4 dígitos"
for user in $(cat $file);do
    idUsuario=$(echo "$user" | sed -rne 's/^[^:]+:[^:]+:([0-9]+).*/\1/p')
    if [ "${#idUsuario}" -eq "4" ]; then
        echo $user
    fi
done
echo " "
echo "4) Usuarios con nombre de 4 caracteres"
for user in $(cat $file);do
    usuario=$(echo "$user" | sed -rne 's/^([^:]+):[^:]+.*$/\1/p')
    if [ "${#usuario}" -eq "4" ]; then
        echo $user
    fi
done