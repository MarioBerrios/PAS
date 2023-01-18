#!/bin/bash

function crear_usuario(){
    FICHERO_USERS=~/asignaturas/pas/pruebas/gestor_usuarios/users.txt
    CARPETA_USUARIO=~/asignaturas/pas/pruebas/gestor_usuarios/homes/$1
    CARPETA_SKEL=~/asignaturas/pas/pruebas/gestor_usuarios/skel

    if [ -d $CARPETA_USUARIO ];then
        echo "El usuario ya existe"
        exit
    fi

    $(echo -e "$1" >> $FICHERO_USERS)
    mkdir $CARPETA_USUARIO
    for f in $(find $CARPETA_SKEL); do
        if [ $f != $CARPETA_SKEL ]; then
            cp $f $CARPETA_USUARIO
        fi
    done
}

if [ $# != 1 ]; then
    echo "No ha llamado al script de manera correcta"
    echo "./ejercicio6.sh <nombre_usuario>"
    exit
fi

NOMBRE_USUARIO=$1

crear_usuario $NOMBRE_USUARIO
