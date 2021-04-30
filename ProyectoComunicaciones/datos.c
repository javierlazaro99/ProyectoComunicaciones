#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include "csv.h"
#include "datos.h"


int cargarValores(DATO** datos, int* size) {

	*datos = NULL;

	CSV_BUFFER* buffer = csv_create_buffer();
	char c[10]; //Buffer para leer cada valor
	int numDatos = 0;

	if (csv_load(buffer, "datosCompletos.csv") == 0) {
		
		for (int i = 0; i < buffer->rows; i++)
		{
			//Cada fila nueva ampliamos nuestro array de estructuras
			*datos = (DATO*)realloc(*datos, (numDatos + 1) * sizeof(DATO));
			if (*datos != NULL) {
				for (int j = 0; j < buffer->width[i]; j++)
				{
					if (csv_get_field(c, 9, buffer, i, j) == 0) {

						switch (j)
						{
						case 0:
							(*datos)[numDatos].ganancia = atof(c);
							break;
						case 1:
							(*datos)[numDatos].numPersonas = atof(c);
							break;
						case 2:
							(*datos)[numDatos].caudal = atof(c);
							break;
						}
					}
					else {
						printf("Error al leer el elemento %d de la fila %d del csv", j, i);
						return(1);
					}
				}
			}
			else {
				printf("Error al alojar memoria para el array");
				return(1);
			}
			

			numDatos++;
		}
	}

	*size = numDatos;
	return(0);
}


//Calcula la ganancia de co2 media en un periodo determinado
double generarGananciaMedia(CSV_BUFFER* bufferLectura, int nMuestras) {

	int suma = 0, cuenta = 0, primerVal = 0;
	double tiempoAct = 0.0, tiempoAnt = 0.0, result = 0.0;
	char co2[10], tiempo[10];
	int totalMediciones = 0;
	double valorMedioTotal = 0.0;

	for (int i = 0; i <= bufferLectura->rows; i++)
	{
		csv_get_field(tiempo, 9, bufferLectura, i, 0);
		csv_get_field(co2, 9, bufferLectura, i, 1);
		tiempoAct = atof(tiempo);

		if (tiempoAct <= tiempoAnt || i == bufferLectura->rows || cuenta == nMuestras) {
			//Toca operar con lo que tenemos hasta ahora
			if (i != 0) {
				result += (double)suma / cuenta - primerVal;
				totalMediciones++;
			}

			printf("Primer valor: %d\n", primerVal);
			printf("Suma: %d\n", suma);
			printf("Cuenta: %d\n", cuenta);
			printf("Result: %lf\n", result);

			//Devolvemos los contadores a su valor original
			suma = 0;
			cuenta = 0;
			primerVal = (int)atof(co2);
		}

		suma += (int)atof(co2);
		cuenta++;
	}

	valorMedioTotal = (double)result / totalMediciones;
	return(valorMedioTotal);
}

//Añade una línea de información al csv completo
int guardaDatosCompletos(CSV_BUFFER* bufferEscritura, char* rutaArchivo, double valorMedioTotal, int numDatos) {

	char gananciaCo2[100];
	char* numPer, * caudal, * nextToken;
	char delim[] = "_.";

	if (snprintf(gananciaCo2, sizeof gananciaCo2, "%lf", valorMedioTotal) < 0) {
		csv_destroy_buffer(bufferEscritura);
		return 1;
	}

	strtok_s(rutaArchivo, delim, &nextToken);
	numPer = strtok_s(NULL, delim, &nextToken);
	caudal = strtok_s(NULL, delim, &nextToken);

	csv_set_field(bufferEscritura, numDatos, 0, gananciaCo2);
	csv_set_field(bufferEscritura, numDatos, 1, numPer);
	csv_set_field(bufferEscritura, numDatos, 2, caudal);

	return 0;
}


//Genera un nombre para el archivo .csv en función de las personas y el caudal
void generarNombreArchivo(char* nombreArchivo, int size, char* numPer, char* caudal) {
	strcpy_s(nombreArchivo, size, "..\\Datos\\dat_");
	strcat_s(nombreArchivo, size, numPer);
	strcat_s(nombreArchivo, size, "_");
	strcat_s(nombreArchivo, size, caudal);
	strcat_s(nombreArchivo, size, ".csv");
}

int guardaDatos(DATO_LEIDO* listaDatos, int numDatos) {

	char tiempo[20], co2[20], numPer[20], caudal[20];
	int comienzo = 0;
	CSV_BUFFER* bufferCreacion = csv_create_buffer();
	DATO_LEIDO* ptr = listaDatos;

	//El caudal y el numero de personas no cambia en un mismo test así que no hace falta mirarlos cada vez
	if (snprintf(numPer, sizeof numPer, "%d", listaDatos->numPersonas) < 0) {
		csv_destroy_buffer(bufferCreacion);
		return(1);
	}
	if (snprintf(caudal, sizeof caudal, "%d", listaDatos->caudal) < 0) {
		csv_destroy_buffer(bufferCreacion);
		return(1);
	}

	char nombreArchivo[100];
	generarNombreArchivo(nombreArchivo, sizeof(nombreArchivo)/sizeof(char), numPer, caudal);

	printf("Guardando datos en la ruta: %s\n", nombreArchivo);

	//Comprobar si existe otro archivo para las mismas condiciones
	if (csv_load(bufferCreacion, nombreArchivo) == 0) {
		comienzo = (int)bufferCreacion->rows;
	}

	for (int i = 0 + comienzo; i < numDatos + comienzo; i++, ptr++)
	{
		if (snprintf(tiempo, sizeof tiempo, "%.1f", ptr->tiempo) < 0) {
			csv_destroy_buffer(bufferCreacion);
			return(1);
		}
		if (snprintf(co2, sizeof co2, "%d", ptr->co2) < 0) {
			csv_destroy_buffer(bufferCreacion);
			return(1);
		}
		
		csv_set_field(bufferCreacion, i, 0, tiempo);
		csv_set_field(bufferCreacion, i, 1, co2);
		csv_set_field(bufferCreacion, i, 2, numPer);
		csv_set_field(bufferCreacion, i, 3, caudal);
	}

	if (csv_save(nombreArchivo, bufferCreacion) == 1) {
		csv_destroy_buffer(bufferCreacion);
		return(1);
	}
	csv_destroy_buffer(bufferCreacion);
	return(0);
}


int combinarDatos(int tiempoMuestra) {

	//Abrir los csv uno a uno
	//Calcular el aumento de co2 medio en el tiempo de muestra
	//Guardar los datos en un csv completo

	int numMuestras = 60 * tiempoMuestra / 2 + 1;
	CSV_BUFFER* bufferLectura;
	CSV_BUFFER* bufferEscritura = csv_create_buffer();

	WIN32_FIND_DATAA file;
	HANDLE fhandle;

	char rutaArchivo[100];
	int numDatos = 0;

	fhandle = FindFirstFileA("..\\Datos\\*.csv", &file);
	if (fhandle == INVALID_HANDLE_VALUE) {
		printf("Invalid File Handle.\n");
		return 1;
	}
	else {
		//Hay al menos 1 archivo en el directorio, lo abrimos y combinamos
		do {
			bufferLectura = csv_create_buffer();
			strcpy_s(rutaArchivo, 100, "..\\Datos\\");
			strcat_s(rutaArchivo, 100, file.cFileName);

			if (csv_load(bufferLectura, rutaArchivo) == 0) {

				printf("Correctamente abierto el archivo: %s\n", file.cFileName);

				//Una vez aquí calculamos la ganacia media de todas las muestras --> Esto es lo que vamos a guardar
				double valorMedioTotal = generarGananciaMedia(bufferLectura, numMuestras);
				printf("Media total: %lf\n", valorMedioTotal);
				
				//Gestionamos la escritura a csv
				if (guardaDatosCompletos(bufferEscritura, rutaArchivo, valorMedioTotal, numDatos) == 1) {
					printf("Error al escribir a datosCompletos.csv");
					csv_destroy_buffer(bufferEscritura);
					return(1);
				}

				numDatos++;
			}
			else {
				printf("Error al abrir el archivo: %s\n", file.cFileName);
			}

			csv_destroy_buffer(bufferLectura);

		} while (FindNextFileA(fhandle, &file) != 0);
	}

	if (csv_save("datosCompletos.csv", bufferEscritura) == 1) {
		csv_destroy_buffer(bufferEscritura);
		return(1);
	}

	csv_destroy_buffer(bufferEscritura);
	return 0;
	
}


//int main() {
//
//	combinarDatos(1);
//	
//	return(0);
//}