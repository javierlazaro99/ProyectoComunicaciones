#include<stdio.h>
#include<string.h>
#include<conio.h>
#include "comunicaciones.h"
#include "datos.h"
#include "regresion.h"
#pragma warning(disable:4996)

//Imprime por pantalla la esctructura del menú principal con el que interactuará el usuario
void mostrarMenuPrincipal() {
	printf("------------- Programa de medicion de CO2 -------------\n");
	printf("[MENU]\n");
	printf("1. Realizar una recopilacion de datos de CO2\n");
	printf("2. Generar el modelo en base a datos guardados\n");
	printf("3. Predecir datos de CO2 usando el modelo\n");
	printf("4. Salir del programa\n");
	printf("Persione una tecla para continuar: ");
}

//Imprime por pantalla la esctructura del menú para lanzar la recopilación de datos
void mostrarMenuTest() {
	printf("------------- Programa de medicion de CO2 -------------\n");
	printf("[MODO DE RECOPILACION DE DATO_LEIDOS]\n");
	printf("Introduzca el tiempo del test (1-30 minutos): ");
}

void mostrarMenuGenerarModelo() {
	printf("------------- Programa de medicion de CO2 -------------\n");
	printf("[GENERACION DEL MODELO EN BASE A DATOS GUARDADOS]\n");
	printf("+----------------------------------------------------------------------------------+\n");
	printf("|  Atencion! Este modo utiliza los datos previamente obtenidos de los experimentos |\n");
	printf("|  La calidad del modelo es equivalente a la calidad de los datos utilizados       |\n");
	printf("+----------------------------------------------------------------------------------+\n");
	printf("Introduzca el intervalo de tiempo para la ganancia de C02 (1-30 minutos): ");
}

void mostrarMenuPrediccionDatos(int numPer, int caudal, int tiempo) {
	printf("------------- Programa de medicion de CO2 -------------\n");
	printf("[PREDICCION DE CO2]\n\n");
	printf("Numero de personas en la sala (+/-): %d \n", numPer);
	printf("Caudal de aire en la sala (m3) (u/d): %d \n", caudal);
	printf("Tiempo de ensayo (min) (h/l): %d \n\n", tiempo);

	printf("Presione la tecla r para reset\n");
	printf("Presione la tecla s para salir\n");
	printf("Presione la tecla espacio para continuar\n\n");
}



int main() {

	//Tiene que tener 3 modos: 
	//	- 1: que haga un experimento y guarde los resultados en un csv 
	//  - 2: otro que coja unos datos guarados, calcule la regresión 
	//  - 3: otro te haga prediciones con lo que le pidas

	HANDLE puerto;
	DATO_LEIDO* listaDatos = (DATO_LEIDO*)calloc(1000, sizeof(DATO_LEIDO));
	int numDatos = 0;
	HANDLE port;

	// ----------------- PARTE VISUAL DEL PROGRAMA -------------------------//
	int tiempoTest, tiempoMuestreo, salir = 0;
	char seleccion;
	char buffTeclado[20];
	double modelo[3];
	memset(modelo, 0, 3*sizeof(double));

	while (salir == 0) {

		mostrarMenuPrincipal();
		seleccion = _getch();

		if ( seleccion == '1') {
			system("cls");
			mostrarMenuTest();

			fgets(buffTeclado, 20, stdin);
			tiempoTest = atol(buffTeclado);
			//scanf("%d", &tiempoTest);

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
			system("cls");
			mostrarMenuGenerarModelo();
			
			fgets(buffTeclado, 20, stdin);
			tiempoMuestreo = atol(buffTeclado);
			
			if (tiempoMuestreo < 1 || tiempoMuestreo > 30) {
				printf("Error, valor fuera del rango permitido\n");
			}
			else {
				combinarDatos(tiempoMuestreo);
				printf("Terminado el proceso de creacion de datos para el modelo\n");
			
				/// Llamar a la función de regresión una vez combinados los datos
				int numIteraciones;
				printf("Introduzca el numero de iteraciones para la regresion (>0): ");
				fgets(buffTeclado, 20, stdin);
				numIteraciones = atol(buffTeclado);
		
				if (numIteraciones > 0) {
					printf("Generando el modelo lineal con los datos cargados...\n");

					regLinMult(modelo, numIteraciones);
					double rmse = calcRmse(modelo);

					printf("\n[MODELO LINEAL RESULTANTE]\n\n");
					printf("- IncCO2 = %+.3f * numPer %+.3f * caudal / 100 %+.3f \n", modelo[2], modelo[1], modelo[0]);
					printf("- Error medio cuadratico del modelo = %.4f\n\n", rmse);
				}
				else {
					printf("Error, valor introducido demasiado bajo\n");
				}
			}

			printf("Pulse ENTER para continuar...\n");
			while (!_kbhit());
		}
		else if (seleccion == '3') {
			system("cls");
			
			if (modelo[2] + modelo[1] + modelo[0] == 0) {
				printf("Error, primero se debe generar un modelo antes de poder hacer predicciones\n");
			}
			else {

				int numPer = 0, caudal = 0, tiempo = 10;
				int salirPrediccion = 0;
				double co2 = 400.0;
				char tecla;

				mostrarMenuPrediccionDatos(numPer, caudal, tiempo);
				
				while (salirPrediccion == 0) {

					if (kbhit()) {
						tecla = _getch();
						if (tecla == 's') {
							salirPrediccion = 1;
						}
						else if (tecla == '+') {
							numPer = numPer + 1;
						}
						else if (tecla == '-') {
							if (numPer > 0) {
								numPer = numPer - 1;
							}
						}
						else if (tecla == 'u') {
							caudal = caudal + 10;
						}
						else if (tecla == 'd') {
							if (caudal > 0) {
								caudal = caudal - 10;
							}
						}
						else if (tecla == 'h') {
							tiempo = tiempo + 1;
						}
						else if (tecla == 'l') {
							if (tiempo > 0) {
								tiempo = tiempo - 1;
							}
						}
						else if (tecla == 'r') {
							co2 = 400.0;
						}

						if (salirPrediccion == 0) {
							system("cls");
							mostrarMenuPrediccionDatos(numPer, caudal, tiempo);
							co2 = predecirCo2(modelo, co2, numPer, caudal, tiempo);
						}
					}					
					
					Sleep(100);

				}
			}
		}
		else if (seleccion == '4') {
			salir = 1;
		}
		
		//Esta línea sólo sirve para borrar consola en Windows
		system("cls");
		
	}
	printf("Terminando el programa...");
	return(0);
}
