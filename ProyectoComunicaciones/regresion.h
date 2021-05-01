#pragma once

#include<stdio.h>
#include "datos.h"

//Función que implementa el algoritmo de descenso de gradiente para calcular una regresión lineal con los datos guardados
//Devuelve un modelo lineal con 3 coeficientes (a0, a1, a2)
void regLinMult(double modelo[3], int numVueltas);

//Función que calcula el error medio cuatrádico dado un modelo lineal y unos datos de contraste
double calcRmse(double modelo[3]);

//Funcion que utiliza el modelo de regresion calculado para predecir la evolucion del CO2 en una sala
//Devuelve el ultimo valor de CO2 calculado
double predecirCo2(double modelo[3], double valCo2Inicial, int numPersonas, int caudal, int vueltas);