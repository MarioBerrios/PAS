#!/bin/bash

file=/etc/group
# Estructura
# nombre:x:gid:lista_de_usuarios
# Los usuarios se encuentran separados por comas (,)

echo "1) Grupos que contengan al menos 1 usuario además del usuario"
for groups in $(cat $file); do
    echo $groups | grep -Eo '^[^:]+:x:[0-9]+:[a-zA-Z0-9,]+$'
done
echo " "
echo "2) Grupos cuyo nombre empiece por u y acabe en s."
for groups in $(cat $file); do
    echo $groups | grep -Eo '^u.*s:x:.*$'
done
echo " "
echo "3) Grupos que no contengan ningún usuario adicional"
for groups in $(cat $file); do
    echo $groups | grep -Eo '^[^:]+:x:[0-9]+:$'
done
echo " "
echo "4) Grupos con GID menor que 100"
for groups in $(cat $file); do
    idGrupo=$(echo $groups | sed -rne 's/^[^:]+:x:([0-9]+).*$/\1/p')
    if [ $idGrupo -lt 100 ]; then
        echo $groups
    fi
done
