#pragma once

#include<stdio.h>

//Estructura que define como será uno de los datos que se lea de puerto serie
typedef struct dato_leido {
    int co2;
    int numPersonas;
    int caudal;
    float tiempo;
} DATO_LEIDO;

//Estructura que define como será uno de los datos que se utilice para la regreson
typedef struct dato {
    double ganancia;
    int numPersonas;
    int caudal;
} DATO;


//Guarda una lista de datos leidos en un archivo .csv, nombra el archivo automaticamente en función de los elemntos del array
int guardaDatos(DATO_LEIDO* listaDatos, int numDatos);

/*Esta función se invoca para abrir todos los csv guardados, recorrerlos y juntarlos en un csv util para 
realizar nuestra regresión, se debe establecer el tiempo de muestra que se va a utilizar (minimo 1)
*/
int combinarDatos(int tiempoMuestra);


/*
Función para cargar los valores del .csv completo y guardarlos en un array dinámico de estructuras de tipo DATO, devuelve el tamaño del array
*/
int cargarValores(DATO** datos, int* size);