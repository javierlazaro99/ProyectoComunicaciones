#include <stdio.h>
#include <math.h>

//Función de cálculo de regresión lineal con descenso de gradiente
void regLin(double* x, double* y, double* res, int size, int numEpoch) {

	double a0 = 0;
	double a1 = 0;
	double alpha = 0.01;

	for (size_t i = 0; i < size * numEpoch; i++)
	{
		size_t index = i % size;
		double predicted = a0 + a1 * x[index];
		double error = predicted - y[index];

		a0 = a0 - alpha * error;
		a1 = a1 - alpha * error * x[index];
	}
	
	res[0] = a0; res[1] = a1;
}

double calcRmse(double* x, double* y, double* lin, int size) {

	double rmse = 0.0;

	for (size_t i = 0; i < size; i++)
	{
		double pred = lin[0] + lin[1] * x[i];
		double error = pred - y[i];
		rmse = rmse + pow(error, 2);
	}

	rmse = sqrt(rmse / size);

	return(rmse);
}


//int main() {
//	double res[2], rmse;
//
//	double x[] = { 1, 2, 4, 3, 5 };
//	double y[] = { 1, 3, 3, 2, 5 };
//	int size = sizeof(x) / sizeof(x[0]);
//
//	regLin(x, y, res, size, 4);
//	rmse = calcRmse(x, y, res, size);
//
//	printf("Funcion obtenida: y = %.5f + %.5f * x \n", res[0], res[1]);
//	printf("RMSE: %.3f \n", rmse);
//
//	return(0);
//}