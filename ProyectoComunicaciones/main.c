#include<stdio.h>
#include "comunicaciones.h"

int main() {

	//Tiene que tener 2 modos: 
	//	- 1: que haga un experimento y guarde los resultados en un csv 
	//  - 2: otro que coja unos datos guarados, calcule la regresión y te haga prediciones con lo que le pidas

	HANDLE puerto;
	DATO* listaDatos = (DATO*)calloc(1000, sizeof(DATO));
	int numDatos = 0;
	HANDLE port;

	if (abrirPuerto(&port) == 0) {
		printf("Error en la creación del puerto");
		return(0);
	}

	if (comenzarExperimento(port, 1) == 0) {
		printf("Error al iniciar experimento");
	}

	if (leerDatosExperimento(port, &listaDatos, &numDatos) == 0) {
		printf("Error durante la comunicación serie");
		return(0);
	}


	return(0);
}
