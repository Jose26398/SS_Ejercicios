#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <list>

using namespace std;

#define SUCESO_LLEGADA_CLIENTE 0
#define SUCESO_SERVIDOR_A 1
#define SUCESO_SERVIDOR_B 2
#define SUCESO_FIN_SIMULACION 3

#define LIBRE 4
#define OCUPADO 5

typedef struct {
	int suceso;
	float tiempo;
} suc;


double reloj;
double tiempo_parada;
double tiempo_clientes_sistema;
int encolaA;
int encolaB;
int clientes_atendidos;
int servidorA;
int servidorB;

list<double>tiempos_llegadas;
list<double>tiempos_medios_estancia;
list<suc> lsuc;

suc sucActual;

double sum = 0;
double sum2 = 0;
bool parar;


bool compare(const suc &s1, const suc &s2)
{ return s1.tiempo < s2.tiempo; }


/* Inserta de forma ordenada un elemento en la lista de sucesos */
void insertar_lsuc(suc n)
{
  lsuc.push_back(n);

  // Mantener ordenada la lista por el tiempo de los sucesos
  lsuc.sort(compare);
}


float generador_exponencial(float media)
{
  float u;
  u = (float) rand();
  u = (float) (u/(RAND_MAX+1.0));
  return(-media*log(1-u));
}


/* Procedimiento temporizacion */
void temporizacion()
{
  sucActual = lsuc.front();
  lsuc.pop_front();
  reloj = sucActual.tiempo;
}


void llegada_cliente()
{
	tiempos_llegadas.push_back(reloj);

	sucActual.suceso = SUCESO_LLEGADA_CLIENTE;
	sucActual.tiempo = reloj + generador_exponencial(1.0);
	insertar_lsuc(sucActual);

	if (servidorA == LIBRE)
	{
		servidorA = OCUPADO;	
		sucActual.suceso = SUCESO_SERVIDOR_A;
		sucActual.tiempo = reloj + generador_exponencial(0.8);
		insertar_lsuc(sucActual);
	}
	else
		encolaA++;
}


void suceso_fin_simulacion()
{
	sucActual.suceso = SUCESO_FIN_SIMULACION;
    sucActual.tiempo = reloj + tiempo_parada;
	insertar_lsuc(sucActual);
}


void llegada_servidorA()
{
	if (encolaA > 0)
	{
		encolaA--;
		sucActual.suceso = SUCESO_SERVIDOR_A;
		sucActual.tiempo = reloj + generador_exponencial(0.8);
		insertar_lsuc(sucActual);
	}
	else
		servidorA = LIBRE;


	if (servidorB == LIBRE)
	{
		servidorB = OCUPADO;
		sucActual.suceso = SUCESO_SERVIDOR_B;
		sucActual.tiempo = reloj + generador_exponencial(1.2);
		insertar_lsuc(sucActual);
	}
	else
		encolaB++;
}



void llegada_servidorB()
{
	clientes_atendidos++;
	tiempo_clientes_sistema += reloj - tiempos_llegadas.front();

	tiempos_llegadas.pop_front();

	if (encolaB > 0)
	{
		encolaB--;
		sucActual.suceso = SUCESO_SERVIDOR_B;
		sucActual.tiempo = reloj + generador_exponencial(1.2);
		insertar_lsuc(sucActual);
	}
	else
		servidorB = LIBRE;
}



/* Procedimiento fin de simulacion */
void fin_simulacion()
{
	parar = true;
	printf("Tiempo clientes: %f y clientes atendidos: %d\n", tiempo_clientes_sistema, clientes_atendidos);
    sum += tiempo_clientes_sistema/clientes_atendidos;
	sum2 += (tiempo_clientes_sistema/clientes_atendidos) * (tiempo_clientes_sistema/clientes_atendidos);
}


/* El generador de informes se encarga de calcular la media y desviacion tipica de los valores obtenidos */
void generador_informes(int simulaciones)
{
	double media = sum / simulaciones,
		   desv = sqrt((sum2 - simulaciones * media * media) / (simulaciones - 1));

	// Mostrar resultados por pantalla
	printf("\n\nINFORME ->");
	printf("\nNúmero de simulaciones: %d", simulaciones);
	printf("\nTiempo medio de estancia: %f \t desviación: %f\n\n", media, desv);
}


/* Procedimiento suceso */
void suceso()
{
  switch(sucActual.suceso)
  	{
	case SUCESO_LLEGADA_CLIENTE: llegada_cliente(); break;
	case SUCESO_SERVIDOR_A: llegada_servidorA(); break;
	case SUCESO_SERVIDOR_B: llegada_servidorB(); break;
	case SUCESO_FIN_SIMULACION: fin_simulacion(); break;
	}
}


void inicializacion()
{
	reloj = 0.0;
	tiempo_parada = 60.0 * 8;
	tiempo_clientes_sistema = 0.0;
	encolaA = 0;
	encolaB = 0;
	clientes_atendidos = 0;
	servidorA = LIBRE;
	servidorB = LIBRE;

	tiempos_llegadas.clear();
	tiempos_medios_estancia.clear();
	lsuc.clear();

	suc sucActual;
	
	llegada_cliente();
	suceso_fin_simulacion();
	
	parar = false;
}



/* Programa principal */
int main(int argc, char *argv[])
{
	int simulaciones;

	if(argc != 2)
	{
		printf("\n\nFormato Argumentos -> <numero_simulaciones>\n\n");
		exit(1);
	}

	sscanf(argv[1],"%d",&simulaciones);

	srand(time(NULL));

	for(int i = 0; i < simulaciones; i++)
	{
		// printf("\nSimulacion %d ...",cont_simu);
		inicializacion();
		while(parar==false)	{
			temporizacion();
			suceso();
		}
	}
	generador_informes(simulaciones);

}
