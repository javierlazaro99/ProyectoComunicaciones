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

//Trata de comunicarse por conexión BT puerto serie con la raspberry
int lanzarComunicacionSerie(DATO** listaDatos, int* numDatos);




