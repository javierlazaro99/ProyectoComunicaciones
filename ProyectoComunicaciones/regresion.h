#pragma once

#include<stdio.h>
#include "datos.h"

//Función que implementa el algoritmo de descenso de gradiente para calcular una regresión lineal con los datos recibidos
//Devuelve un modelo lineal con 3 coeficientes (a0, a1, a2)
void regLinMult(DATO* listaDatos, double modelo[3], int numDatos, int numVueltas);

//Función que calcula el error medio cuatrádico dado un modelo lineal y unos datos de contraste
double calcRmse(DATO* listaDatos, double modelo[3], int numDatos);
