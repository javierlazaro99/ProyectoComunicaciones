#include <stdio.h>
#include <math.h>
#include "datos.h"


void regLinMult(DATO* listaDatos, double modelo[3], int numDatos, int numVueltas) {

	double w0 = 0, w1 = 0, w2 = 0;
	
	//Cuanto menor es alfa menor es el cambio entre iteraciones --> Se puede modificar para intentar obtener mejores resultados
	double alfa = 0.01; 
	double hipotesis = 0, error = 0;


	for (int i = 0; i < numDatos * numVueltas; i++)
	{
		int indice = i % numDatos;
		//Realizar la hipóteis y calcular el error
		hipotesis = w2 * listaDatos[indice].numPersonas + w1 * (listaDatos[indice].caudal / 100) + w0;
		error = listaDatos[indice].ganancia - hipotesis;

		//Actualizar los pesos de los ceoficientes
		w2 = w2 + alfa * error * listaDatos[indice].numPersonas;
		w1 = w1 + alfa * error * (listaDatos[indice].caudal / 100);
		w0 = w0 + alfa * error;

		if (i % numVueltas == 0)
		{
			printf("Iteracion %d: ganancia = %.3f * numPer + %.3f * caudal / 100 + %.3f \n", i, w2, w1, w0);
			printf("Valor del error: %.5f\n", error);
		}
	}

	modelo[0] = w0;
	modelo[1] = w1;
	modelo[2] = w2;
}


double calcRmse(DATO * listaDatos, double modelo[3], int numDatos) {

	double rmse = 0.0;
	double hipotesis, error;

	for (int i = 0; i < numDatos; i++)
	{
		hipotesis = modelo[2] * listaDatos[i].numPersonas + modelo[1] * (listaDatos[i].caudal / 100) + modelo[0];
		error = listaDatos[i].ganancia - hipotesis;
		rmse = rmse + pow(error, 2);
	}

	rmse = sqrt(rmse / numDatos);

	return(rmse);
}


int main() {
	
	DATO* listaDatos = NULL;
	int size = 0;
	double resultado[3];

	cargarValores(&listaDatos, &size);

	regLinMult(listaDatos, resultado, size, 100);

	double rmse = calcRmse(listaDatos, resultado, size);

	printf("RMSE: %.4f\n", rmse);

	printf("RESULTADO: ganancia = %.3f * numPer + %.3f * caudal / 100 + %.3f \n", resultado[2], resultado[1], resultado[0]);

	return(0);
}