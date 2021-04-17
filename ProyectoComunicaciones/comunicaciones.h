#pragma once

#include<stdio.h>
#include<Windows.h>

//Estructura que define como será uno de los datos que registre
typedef struct dato {
    int co2;
    int numPersonas;
    int caudal;
    float tiempo;
} DATO;

//Combina 4 bytes en un unsigned int
unsigned int combinarAInt(unsigned char* bytes, int inicio);

//Combina 4 bytes a un float
void combinarAFloat(unsigned char* bytes, int inicio, float* f);

//Combina el buffer leído por serial y lo convierte en una estructura de tipo DATO
int combinarBuffer(DATO* miDato, char* buffer, int tamBuffer, int numSep, char fin);

//Abre el puerto correspondiente a la conexión BT serial, lo configura y devuelve su HANDLE
int abrirPuerto(HANDLE* port);

//Escribe un mensaje por el puerto serie correspondinte para indicar a la raspberry que comience el experimento
int comenzarExperimento(HANDLE port);

//Trata de comunicarse por conexión BT puerto serie con la raspberry
int leerDatosExperimento(HANDLE port, DATO** listaDatos, int* numDatos);




