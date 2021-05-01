#pragma once

#include<stdio.h>
#include "datos.h"

//Funci�n que implementa el algoritmo de descenso de gradiente para calcular una regresi�n lineal con los datos guardados
//Devuelve un modelo lineal con 3 coeficientes (a0, a1, a2)
void regLinMult(double modelo[3], int numVueltas);

//Funci�n que calcula el error medio cuatr�dico dado un modelo lineal y unos datos de contraste
double calcRmse(double modelo[3]);

//Funcion que utiliza el modelo de regresion calculado para predecir la evolucion del CO2 en una sala
//Devuelve el ultimo valor de CO2 calculado
double predecirCo2(double modelo[3], double valCo2Inicial, int numPersonas, int caudal, int vueltas);