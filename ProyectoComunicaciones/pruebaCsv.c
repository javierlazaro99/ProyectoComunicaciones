#include<stdio.h>
#include "csv.h"

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


//int main() {
//
//	double* x;
//	double* y;
//	int size;
//
//	CSV_BUFFER* buffer = csv_create_buffer();
//	csv_load(buffer, "test.csv");
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