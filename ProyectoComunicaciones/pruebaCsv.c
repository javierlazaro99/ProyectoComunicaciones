#include<stdio.h>
#include "csv.h"
#include "datos.h"


void print_buffer(CSV_BUFFER* buffer) {

	char c[10];

	for (int i = 0; i < buffer->rows; i++)
	{
		for (int j = 0; j < buffer->width[i]; j++)
		{
			csv_get_field(c, 9, buffer, i, j);
			printf("%s\n", c);
		}
	}
}

//Carga 2 arrays de doubles con los valores de x e y del csv que se haya leido
//Devuelve el valor del tamaño que tendrán dichos arrays en memoria
int cargar_valores(double** x, double** y, CSV_BUFFER* buffer) {
	*y = NULL;
	*x = NULL;

	char c[10]; //Buffer para leer cada valor
	int size = 0;
	
	for (int i = 1; i < buffer->rows; i++)
	{
		size++;
		for (int j = 0; j < buffer->width[i]; j++)
		{
			csv_get_field(c, 9, buffer, i, j);
			
			if (j == 0) {
				*y = (double*)realloc(*y, size*sizeof(double));
				if (*y != NULL) {
					(*y)[size - 1] = atof(c);
				}
				else {
					printf("Error al hacer realloc para y");
					return(0);
				}
			}
			else if (j == 1) {
				*x = (double*)realloc(*x, size*sizeof(double));
				if (*x != NULL) {
					(*x)[size - 1] = atof(c);
				}
				else {
					printf("Error al hacer realloc para x");
					return(0);
				}
			}
			else {
				return (0);
			}
		}
	}
	return(size);
}


int guardaDatos(DATO* listaDatos, int numDatos) {

	char tiempo[20], co2[20], numPer[20], caudal[20];
	int comienzo = 0;
	CSV_BUFFER* bufferCreacion = csv_create_buffer();
	DATO* ptr = listaDatos;

	//El caudal y el numero de personas no cambia en un mismo test así que no hace falta mirarlos cada vez
	if (snprintf(numPer, sizeof numPer, "%d", listaDatos->numPersonas) < 0) return(1);
	if (snprintf(caudal, sizeof caudal, "%d", listaDatos->caudal) < 0) return(1);

	char nombreArchivo[100];
	strcpy_s(nombreArchivo, 100, "..\\Datos\\dat_");
	strcat_s(nombreArchivo, 100, numPer);
	strcat_s(nombreArchivo, 100, "_");
	strcat_s(nombreArchivo, 100, caudal);
	strcat_s(nombreArchivo, 100, ".csv");

	printf("Guardando datos en la ruta: %s\n", nombreArchivo);

	//Comprobar si existe otro archivo para las mismas condiciones
	if (csv_load(bufferCreacion, nombreArchivo) == 0) {
		comienzo = (int)bufferCreacion->rows;
	}

	for (int i = 0 + comienzo; i < numDatos + comienzo; i++, ptr++)
	{
		if(snprintf(tiempo, sizeof tiempo, "%.1f", ptr->tiempo) < 0) return(1);
		if (snprintf(co2, sizeof co2, "%d", ptr->co2) < 0) return(1);
		
		csv_set_field(bufferCreacion, i, 0, tiempo);
		csv_set_field(bufferCreacion, i, 1, co2);
		csv_set_field(bufferCreacion, i, 2, numPer);
		csv_set_field(bufferCreacion, i, 3, caudal);
	}

	if (csv_save(nombreArchivo, bufferCreacion) == 1) return(1);
	csv_destroy_buffer(bufferCreacion);
	return(0);
}


//int main() {
//
//	double* x;
//	double* y;
//	int size;
//
//	CSV_BUFFER* buffer = csv_create_buffer();
//	csv_load(buffer, "..\\Datos\\dat_1_100.csv");
//
//	size_t rows = buffer->rows;
//
//	size = cargar_valores(&x, &y, buffer);
//
//	print_buffer(buffer);
//
//	CSV_BUFFER* bufferCreacion = csv_create_buffer();
//	
//	csv_set_field(bufferCreacion, 0, 0, "test");
//	csv_set_field(bufferCreacion, 0, 1, "hola");
//	csv_set_field(bufferCreacion, 1, 0, "hey");
//	csv_set_field(bufferCreacion, 1, 1, "chao");
//
//	print_buffer(bufferCreacion);
//
//	csv_save("testSave.csv", bufferCreacion);
//
//	csv_destroy_buffer(buffer);
//	csv_destroy_buffer(bufferCreacion);
//	
//
//
//	return(0);
//}