#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
	double parte_real, parte_imaginaria;
}complejo_t;

complejo_t* crear_complejo(double,double);
double obtener_parte_real(complejo_t*);
double obtener_parte_imaginaria(complejo_t*);
complejo_t* elevar_complejo_al_cuadrado(complejo_t*);
complejo_t* sumar_dos_complejos(complejo_t*,complejo_t*);
double calcular_modulo(complejo_t*);
void destruir_complejo(complejo_t*);
complejo_t* calcular_siguiente_iteracion(complejo_t*,complejo_t*);
//int* calcular_intensidades(int*[],complejo_t*);
void calcular_intensidades(complejo_t*);

int ancho = 640;
int alto = 480;
double altura_maxima = 1;
double altura_minima = -1;
double ancho_minimo = -1;
double ancho_maximo = 1;
double step_altura = 1;
double step_ancho = 1;
/* Todos numeros placeholders*/

complejo_t* crear_complejo(double parte_real, double parte_imaginaria){
	complejo_t* complejo = malloc(sizeof(complejo_t));
	if (!complejo)
		return NULL;
	complejo->parte_real = parte_real;
	complejo->parte_imaginaria = parte_imaginaria;
	return complejo;
}

double obtener_parte_real(complejo_t* complejo){
	return (complejo->parte_real);
}

double obtener_parte_imaginaria(complejo_t* complejo){
	return (complejo->parte_imaginaria);
}

complejo_t* elevar_complejo_al_cuadrado(complejo_t* complejo){
	double parte_real = complejo->parte_real;
	double parte_imaginaria = complejo->parte_imaginaria;
	double parte_real_cuadrado = ((parte_real*parte_real)-(parte_imaginaria*parte_imaginaria));
	double parte_imaginaria_cuadrado = (2*parte_real*parte_imaginaria);
	complejo_t* cuadrado = crear_complejo(parte_real_cuadrado,parte_imaginaria_cuadrado);
	return cuadrado;
}

/*
complejo_t* elevar_complejo_al_cuadrado(complejo_t* complejo){
	double parte_real = complejo->parte_real;
	double parte_imaginaria = complejo->parte_imaginaria;
	double parte_real_cuadrado = ((parte_real*parte_real)-(parte_imaginaria*parte_imaginaria));
	double parte_imaginaria_cuadrado = (2*parte_real*parte_imaginaria);
	complejo->parte_real = parte_real_cuadrado;
	complejo->parte_imaginaria = parte_imaginaria_cuadrado;
	return complejo;
}
Esta version del cuadrado pisa el complejo pasado por parametro
Desventaja: pisa el complejo pasado por parametro
Ventaja: ahorra memoria*/

complejo_t* sumar_dos_complejos(complejo_t* complejo1,complejo_t* complejo2){
	double parte_real_suma = (complejo1->parte_real)+(complejo2->parte_real);
	double parte_imaginaria_suma = (complejo1->parte_imaginaria)+(complejo2->parte_imaginaria);
	complejo_t* suma = crear_complejo(parte_real_suma,parte_imaginaria_suma);
	return suma;
}

/*
complejo_t* sumar_dos_complejos(complejo_t* complejo1,complejo_t* complejo2){
	double parte_real_suma = (complejo1->parte_real)+(complejo2->parte_real);
	double parte_imaginaria_suma = (complejo1->parte_imaginaria)+(complejo2->parte_imaginaria);
	complejo1->parte_real = parte_real_suma;
	complejo1->parte_imaginaria = parte_imaginaria_suma;
	return complejo1;
}

Esta version de la suma pisa el primer complejo pasado por parametro
Desventaja: pisa el primer complejo pasado por parametro
Ventaja: ahorra memoria*/

double calcular_modulo(complejo_t* complejo){
	double parte_real = obtener_parte_real(complejo);
	double parte_imaginaria = obtener_parte_imaginaria(complejo);
	return sqrt((parte_real*parte_real)+(parte_imaginaria*parte_imaginaria));
}

void destruir_complejo(complejo_t* complejo){
	free(complejo);
}

complejo_t* calcular_siguiente_iteracion(complejo_t* complejo, complejo_t* constante){
	return sumar_dos_complejos(elevar_complejo_al_cuadrado(complejo),constante);
}

int main(void){
	//Procesar argumentos
	int* arreglo_de_intensidades = malloc(ancho*alto*sizeof(int));
	if (!arreglo_de_intensidades)
		return -1;	//Manejar error
	complejo_t* constante = crear_complejo(0,0);
	calcular_intensidades(constante);
	if (!arreglo_de_intensidades)
		return -1; 	//Manejar error
	//Crear la imagen
	free(arreglo_de_intensidades);
	return 0;
}


void calcular_intensidades(/*int* [arreglo],*/complejo_t* constante){ //otros parametros
	//calcular alturas y anchos minimo y maximo de acuerdo con los argumentos
	//step (paso) creo que es ancho_region (defaut 2) dividido ancho (default 640)
	complejo_t* pixel;
	//int indice = 0;
	for (int j=altura_maxima; j>altura_minima; j-=step_altura){
		for (int i=ancho_minimo; i<ancho_maximo; i+=step_ancho){
			pixel = crear_complejo(i,j);
			if(!pixel)
				return; //Manejar error
			int cantidad_iteraciones = 0;
			while ((calcular_modulo(pixel)<1)&&(cantidad_iteraciones<255)){
				pixel = calcular_siguiente_iteracion(pixel,constante);
				cantidad_iteraciones++;
			}
			//arreglo[indice] = cantidad_iteraciones;
			//indice++;
		}
	}
	destruir_complejo(pixel);
	return;
}
