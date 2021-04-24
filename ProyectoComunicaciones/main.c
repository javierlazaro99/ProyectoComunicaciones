#include<stdio.h>
#include<string.h>
#include<conio.h>
#include "comunicaciones.h"
#include "datos.h"
#pragma warning(disable:4996)

//Imprime por pantalla la esctructura del menú principal con el que interactuará el usuario
void mostrarMenuPrincipal() {
	printf("------------- Programa de medicion de CO2 -------------\n");
	printf("[MENU]\n");
	printf("1. Realizar una recopilacion de datos de CO2\n");
	printf("2. Realizar una prediccion con los datos historicos\n");
	printf("3. Salir del programa\n");
	printf("Persione una tecla para continuar: ");
}

//Imprime por pantalla la esctructura del menú para lanzar la recopilación de datos
void mostrarMenuTest() {
	printf("------------- Programa de medicion de CO2 -------------\n");
	printf("[MODO DE RECOPILACION DE DATOS]\n");
	printf("Introduzca el tiempo del test (5-30 minutos): ");
}


int main() {

	//Tiene que tener 2 modos: 
	//	- 1: que haga un experimento y guarde los resultados en un csv 
	//  - 2: otro que coja unos datos guarados, calcule la regresión y te haga prediciones con lo que le pidas

	HANDLE puerto;
	DATO* listaDatos = (DATO*)calloc(1000, sizeof(DATO));
	int numDatos = 0;
	HANDLE port;

	// ----------------- PARTE VISUAL DEL PROGRAMA -------------------------//
	int tiempoTest, salir = 0;
	char seleccion;

	while (salir == 0) {

		mostrarMenuPrincipal();
		seleccion = _getch();

		if ( seleccion == '1') {
			system("cls");
			mostrarMenuTest();
			scanf("%d", &tiempoTest);

			if (tiempoTest < 1 || tiempoTest > 30) {
				printf("Error, valor fuera del rango permitido\n");
			}
			else {
				printf("Tiempo del test establecido a %d minutos\n", tiempoTest);
				printf("Comenzar test? (s/n)\n");
				if (_getch() == 's') {
					system("cls");
					//Se abre el puerto y se intenta lanzar el experimento con los valores introducidos por el usuario
					if (abrirPuerto(&port)) {
						printf("Error en la creación del puerto");
						return(0);
					}
					if (comenzarExperimento(port, tiempoTest)) {
						printf("Error al iniciar experimento");
					}
					if (leerDatosExperimento(port, &listaDatos, &numDatos)) {
						printf("Error durante la comunicación serie");
						return(0);
					}

					printf("Desea guardar los datos obtenidos? (s/n)\n");
					if (_getch() == 's') {
						if (guardaDatos(listaDatos, numDatos) == 0) {
							printf("Datos guardados correctamente!\n");
						}
						else {
							printf("Error al tratar de guardar los datos\n");
						}
					}
					

				}
			}
			printf("Pulse ENTER para continuar...\n");
			while (!_kbhit());
		}
		else if (seleccion == '2') {
			printf("TECLA 2 PULSADA\n");
		}
		else if (seleccion == '3') {
			salir = 1;
		}
		
		//Esta línea sólo sirve para borrar consola en Windows
		system("cls");
		
	}
	printf("Terminando el programa...");
	return(0);
}
