#pragma once

#include<stdio.h>
#include<Windows.h>
#include "datos.h"



//Abre el puerto correspondiente a la conexión BT serial, lo configura y devuelve su HANDLE
int abrirPuerto(HANDLE* port);

//Escribe un mensaje por el puerto serie correspondinte para indicar a la raspberry que comience el experimento
int comenzarExperimento(HANDLE port, int time);

//Trata de comunicarse por conexión BT puerto serie con la raspberry
int leerDatosExperimento(HANDLE port, DATO_LEIDO** listaDatos, int* numDatos);




