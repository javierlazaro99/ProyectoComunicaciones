#include <stdio.h>
#include <math.h>
#include "datos.h"


void regLinMult(double modelo[3], int numVueltas) {

	//Cargamos los valores para realizar la regresión
	DATO* listaDatos = NULL;
	int numDatos = 0;
	cargarValores(&listaDatos, &numDatos);

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

		/*if (i % numVueltas == 0)
		{
			printf("Iteracion %d: ganancia = %+.3f * numPer %+.3f * caudal / 100 %+.3f \n", i, w2, w1, w0);
			printf("Valor del error: %.5f\n", error);
		}*/
	}

	modelo[0] = w0;
	modelo[1] = w1;
	modelo[2] = w2;
}


double calcRmse(double modelo[3]) {

	//Cargamos los valores para realizar la regresión
	DATO* listaDatos = NULL;
	int numDatos = 0;
	cargarValores(&listaDatos, &numDatos);

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


double predecirCo2(double modelo[3], double valCo2Inicial, int numPersonas, int caudal, int vueltas) {

	int tiempo = 0;
	double co2 = valCo2Inicial;
	
	printf("Predicciones para los siguientes %d minutos: \n", vueltas);
	for (int i = 0; i < vueltas; i++)
	{
		co2 = co2 + modelo[2] * numPersonas + modelo[1] * caudal / 100 + modelo[0];
		printf("Minuto %d: CO2 = %lf ppm\n", ++tiempo, co2);
	}

	return(co2);
}

//int main() {
//	
//	
//	double resultado[3];
//
//	
//
//	regLinMult(resultado, 100);
//
//	double rmse = calcRmse(resultado);
//
//	printf("RMSE: %.4f\n", rmse);
//
//	printf("RESULTADO: ganancia = %.3f * numPer + %.3f * caudal / 100 + %.3f \n", resultado[2], resultado[1], resultado[0]);
//
//	return(0);
//}