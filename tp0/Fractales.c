#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
	double parte_real, parte_imaginaria;
}complejo_t;


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

complejo_t* sumar_dos_complejos(complejo_t* complejo1,complejo_t* complejo2){
	double parte_real_suma = (complejo1->parte_real)+(complejo2->parte_real);
	double parte_imaginaria_suma = (complejo1->parte_imaginaria)+(complejo2->parte_imaginaria);
	complejo_t* suma = crear_complejo(parte_real_suma,parte_imaginaria_suma);
	return suma;
}

double calcular_modulo(complejo_t* complejo){
	double parte_real = obtener_parte_real(complejo);
	double parte_imaginaria = obtener_parte_imaginaria(complejo);
	return sqrt((parte_real*parte_real)+(parte_imaginaria*parte_imaginaria));
}

void destruir_complejo(complejo_t* complejo){
	free(complejo);
}

complejo_t* calcular_siguiente_iteracion(complejo_t* complejo, complejo_t* constante){
	complejo_t* cuadrado = elevar_complejo_al_cuadrado(complejo);
	complejo_t* suma = sumar_dos_complejos(cuadrado,constante);
	destruir_complejo(cuadrado);
	return suma;
}

void calcular_intensidades(int* arreglo_de_intensidades, int ancho_pixeles, int alto_pixeles,
							double ancho_complejos, double alto_complejos,
							complejo_t* constante, complejo_t* centro){
	

	double altura_maxima = centro->parte_imaginaria + alto_complejos/2;
	double ancho_minimo = centro->parte_real - ancho_complejos/2;
	double step_altura = alto_complejos / alto_pixeles;
	double step_ancho = ancho_complejos / ancho_pixeles;

	complejo_t* pixel;
	complejo_t* pixelNuevo;
	int indice = 0;
	for (double j=0; j<alto_pixeles; j++){
		for (double i=0; i<ancho_pixeles; i++){
			pixel = crear_complejo(ancho_minimo + i*step_ancho,altura_maxima - j*step_altura);
			if(!pixel)
				return; //Manejar error
			int cantidad_iteraciones = 0;
			while ((calcular_modulo(pixel)<=2)&&(cantidad_iteraciones<255)){
				pixelNuevo = calcular_siguiente_iteracion(pixel,constante);
				destruir_complejo(pixel);
				pixel = pixelNuevo;
				cantidad_iteraciones++;
			}
			arreglo_de_intensidades[indice] = cantidad_iteraciones;
			indice++;
			destruir_complejo(pixel);
		}
	}
	return;
}

void crear_archivo_pgm(const char *filename, int ancho_pixeles, int alto_pixeles, 
						int* arreglo_de_intensidades){
	FILE* fp = fopen(filename, "w");
	fprintf(fp, "%s\n", "P2");
	fprintf(fp, "%s\n", "# Trabajo practico de Organizacion de Computadoras");
	fprintf(fp, "%d %d\n", ancho_pixeles, alto_pixeles);
	fprintf(fp, "%s\n", "255");
	int indice = 0;
	for (int i = 0; i < ancho_pixeles; i++){
		for (int i = 0; i < alto_pixeles; ++i){
			fprintf(fp, "%d\n", arreglo_de_intensidades[indice]);
			indice++;
		}
	}
	fclose(fp);
}

int main(void){

	int ancho_pixeles = 640;
	int alto_pixeles = 480;
	double ancho_complejos = 2;
	double alto_complejos = 2;
	complejo_t* centro = crear_complejo(0,0);
	complejo_t* constante = crear_complejo(-0.726895347709114071439,0.188887129043845954792);
	char* filename = "imagen.pgm";

	//Procesar argumentos
	int* arreglo_de_intensidades = malloc(ancho_pixeles*alto_pixeles*sizeof(int));
	if (!arreglo_de_intensidades)
		return -1;	//Manejar error
	
	calcular_intensidades(arreglo_de_intensidades, ancho_pixeles, alto_pixeles, ancho_complejos, alto_complejos, 
							constante, centro);
	if (!arreglo_de_intensidades)
		return -1; 	//Manejar error

	crear_archivo_pgm(filename, ancho_pixeles, alto_pixeles, arreglo_de_intensidades);

	free(arreglo_de_intensidades);
	destruir_complejo(centro);
	destruir_complejo(constante);
	return 0;
}