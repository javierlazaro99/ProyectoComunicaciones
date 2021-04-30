#pragma once

#include<stdio.h>
#include "datos.h"

//Funci�n que implementa el algoritmo de descenso de gradiente para calcular una regresi�n lineal con los datos recibidos
//Devuelve un modelo lineal con 3 coeficientes (a0, a1, a2)
void regLinMult(DATO* listaDatos, double modelo[3], int numDatos, int numVueltas);

//Funci�n que calcula el error medio cuatr�dico dado un modelo lineal y unos datos de contraste
double calcRmse(DATO* listaDatos, double modelo[3], int numDatos);
