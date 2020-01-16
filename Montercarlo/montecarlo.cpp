#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace std;

int a = 2000, b = 3000, c = 2600;

float uniforme(){
	float u;
	u = (float) random();
	u = u/(float)(RAND_MAX+1.0);
	return u;
}


int generademanda(){
	float u, x;
	u = uniforme();
	
	if (u < (c-a)/(b-a))
		x = a+sqrt((b-a)*(c-a)*u);
	else
		x = b-sqrt((b-a)*(b-c)*(1-u));
	
	return (int) x; //se convierte a entero porque es la demanda de huevos de pascua
}



int main(int argc, char* argv[])
{
	double x = 0.6,	 	// Ganancia por unidad vendida
		   y1 = 0.25, 	// Pérdida por unidad no vendida con chocolate comprado en agosto
		   y2 = 0.375;	// Pérdida por unidad no vendida con chocolate comprado en diciembre
	
	int veces = 10000;	
	int demanda, ganancia, s_maxima;
	double ganancia_maxima = 0;

	srand(time(NULL)); //Inicializa el generador de numeros pseudoaleatorios

	// Ejecutar modelo de Montecarlo
	for (int s = a; s <= b; s++){
		double sum = 0.0, sum2 = 0.0;
		
		for (int i = 0; i < veces; i++){
			demanda = generademanda();

			if (s > demanda)
				ganancia = demanda*x - s*y1;

			else
				ganancia = demanda*x - (s*y1 + (demanda-s)*y2);

			sum += ganancia;
			sum2 += ganancia*ganancia;
		}
		
		// Obtener ganancia media y desviacion tipica
		double ganancia_esperada = sum/veces,
			   desviacion = sqrt((sum2-veces*ganancia_esperada*ganancia_esperada)/(veces - 1));

		if (ganancia_esperada > ganancia_maxima){
			ganancia_maxima = ganancia_esperada;
			s_maxima = s;
		}

		printf("s: %d, ganancia: %f, desv: %f\n", s, ganancia_esperada, desviacion);
	}

	printf("\nValor de x: %f, valor de y1: %f, valor de y2: %f, numero de veces: %d", x, y1, y2, veces);
	printf("\nValor maximo de ganancia: %f || s --> %d\n", ganancia_maxima, s_maxima);
	return 0;
} 
