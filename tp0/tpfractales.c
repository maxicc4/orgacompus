#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

int calcular_intensidades(int* arreglo_de_intensidades, int ancho_pixeles, int alto_pixeles,
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
			if(!pixel){
				perror("Error al crear representacion compleja del pixel");
				return -1;
			}
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
	return 0;
}

int crear_archivo_pgm(const char *filename, int ancho_pixeles, int alto_pixeles, 
						int* arreglo_de_intensidades){
	int res;
	FILE* fp;
	if (strcmp(filename, "-") == 0){
		fp = stdout;
	} else {
		fp = fopen(filename, "w");
	}
	if (fp == NULL){
		perror("Error al abrir archivo");
		return -1;
    }
	res = fprintf(fp, "%s\n%s\n%d %d\n%s\n", 
		"P2", 
		"# Trabajo practico de Organizacion de Computadoras", 
		ancho_pixeles, alto_pixeles, 
		"255");
	if (res < 0){
		perror("Error al escribir en el archivo");
		return -1;
	}
	int indice = 0;
	for (int i = 0; i < ancho_pixeles; i++){
		for (int i = 0; i < alto_pixeles; ++i){
			res = fprintf(fp, "%d\n", arreglo_de_intensidades[indice]);
			indice++;
			if (res < 0){
				perror("Error al escribir en el archivo");
				return -1;
			}
		}
	}
	fclose(fp);
	return 0;
}

const char* lecturaArgumentos(int argc, char *argv[],int* anchox,int* altoy,double* h_rec,double* w_rec,double* center_x,double* center_y,
	double* seed_x,double* seed_y,const char* archivo){
	int i=0;
	// pongo valores por defecto 
	int ancho = 640;
	int alto = 480;
	*center_x = 0;
	*center_y = 0;
	*seed_x =-0.726895347709114071439;
	*seed_y = 0.188887129043845954792;
 	archivo = "c";
	*anchox = ancho;	
	*altoy = alto;
	archivo = "-";
	for (i=1; i< argc; i++) // vamos recorriendo los argumentos
	{
		if(!strcmp(argv[i], "-r") || !strcmp(argv[i], "--resolution"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				if( strcspn (argv[i+1], "x") > 0 && strcspn (argv[i+1], "x") < strlen(argv[i+1]) )	 					//entra sihay almenos 1 x y no es al inicio
				{
				   char str1[20];
				   memset(&str1[0], 0, sizeof(str1));
				   int longitud = strcspn (argv[i+1], "x");
				   strncat(str1, argv[i+1], longitud);
				   const char separador = 'x';
				   char *valor2;
				   valor2 = strchr(argv[i+1], separador);
				   valor2 = valor2 +1;
				   if(atoi(str1) > 40 && atoi(valor2) > 40)
				   {
				   	*anchox = atoi(str1);
					*altoy = 	atoi(valor2);
				   	//printf("ancho:%i ",*anchox );
					//printf("alto:%i ",*altoy );
				   }
				}

			}
		}
		
		if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--center") || !strcmp(argv[i], "-s") || !strcmp(argv[i], "--seed"))
		{	
			char valor1[20];
			char *valor2;
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				argv[i+1] = argv[i+1] + 1;
				memset(&valor1[0], 0, sizeof(valor1));
					//ACORDARSE DE VOLVER AL RESTAR AL FINAL ANTES DE HACER EL ATOF
				if(  strcspn (argv[i+1], "+") < strlen(argv[i+1])  
					|| ( strcspn (argv[i+1], "-") < strlen(argv[i+1]) ) )	 					//entra 				sihay almenos 1 + o un - ,y no es al inicio
				{
					if(strcspn (argv[i+1], "+") < strlen(argv[i+1]))
					{
						
						int longitud = strcspn (argv[i+1], "+");
					        strncat(valor1, argv[i+1]-1, longitud+1);
						const char separador = '+';
				  		
				  		valor2 = strchr(argv[i+1], separador);
				  	}
					if(strcspn (argv[i+1], "-") < strlen(argv[i+1]))
					{
						//const char valor1[20];
						int longitud = strcspn (argv[i+1], "-");
					        strncat(valor1, argv[i+1]-1, longitud+1);
						const char separador = '-';
				  		valor2 = strchr(argv[i+1], separador);
					}
				}
				//printf("%c",valor2[strlen(valor2)-1]);						
                        	argv[i+1] = argv[i+1]-1; 
				if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--center"))
				{
					if( ( (atof(valor1) != 0) || (valor1[0] == '0') ) && ((atof(valor2) != 0) || (valor2[0] == '0')) )
					{					
						
						if(valor2[strlen(valor2)-1] == 'i')					
						{
					        	*center_x = atof(valor1);
							*center_y = atof(valor2);
							//printf(" centerx:%f",*center_x );
							//printf("centery:%f",*center_y );
						}
					}
				}
				if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--seed"))
				{
					if( ( (atof(valor1) != 0) || (valor1[0] == '0') ) && ((atof(valor2) != 0) || (valor2[0] == '0')) )
					{
					   	if(valor2[strlen(valor2)-1] == 'i')	
						{
							*seed_x = atof(valor1);
							*seed_y = atof(valor2);
						   	//printf("seedx:%f",*seed_x );
							//printf("seedy:%f",*seed_y );
						}
					}
				}       
			}
		}
		
		if(!strcmp(argv[i], "-w") || !strcmp(argv[i], "--width"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				if( atof(argv[i+1]) >= 2)
				{
					*w_rec = atof(argv[i+1]);
				}
			}
		}
		if(!strcmp(argv[i], "-H") || !strcmp(argv[i], "--height"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				if(atof(argv[i+1]) >= 2) // validamos q sea mayor a 1 quitamos la chance q sea negativo
				{
					*h_rec = atof(argv[i+1]);
				}
				
			}
		}
		if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
		{	
			
			if( (argv[i+1] != NULL) && (strcmp(argv[i+1], "-" ) ) )// si existe el siguiente
			{	
				archivo = argv[i+1];// debemos ver si es un archivo valido
				//printf("%s",archivo);
			}
			
		}					
        }
	return archivo;
}


int main(int argc, char *argv[]){

	int ancho_pixeles = 640;
	int alto_pixeles = 480;
	double ancho_complejos = 2;
	double alto_complejos = 2;
	double center_x = 0;
	double center_y = 0;
	double seed_x = 0;
	double seed_y = 0;
	const char* filename = "-"; 
	// retorna '-' salida por consola, o el nombre del archivo
	filename = lecturaArgumentos(argc,argv,&ancho_pixeles,&alto_pixeles,&ancho_complejos,&alto_complejos,
	&center_x,&center_y,&seed_x,&seed_y,filename);	
	// esto es para las pruebas de validacion despues se saca
	printf("anchoresolucion:%i\n ",ancho_pixeles );
	printf("altoresolucion:%i\n ",alto_pixeles );
	printf("anchocomplejo:%f\n ",ancho_complejos );
	printf("altodescomplejo:%f\n ",alto_complejos );
	printf("anchocentro:%f\n ",center_x );
	printf("altocentro:%f\n ",center_y );
	printf("anchoseed:%f\n ",seed_x );
	printf("altoseed:%f\n ",seed_y );
	printf("nombrearchivo:%s\n ",filename );
	
	complejo_t* centro = crear_complejo(center_x,center_y);
	complejo_t* constante = crear_complejo(seed_x,seed_y);
	

	int res = 0;
	//Procesar argumentos
	int* arreglo_de_intensidades = malloc(ancho_pixeles*alto_pixeles*sizeof(int));
	if (!arreglo_de_intensidades){
		perror("Error al reservar memoria para la intensidad de pixeles");
		return -1;
	}

	res = calcular_intensidades(arreglo_de_intensidades, ancho_pixeles, alto_pixeles, ancho_complejos, alto_complejos, 
							constante, centro);
	if (res == -1){
		perror("Error al calcular intensidades de los pixeles");
		return -1;
	}

	res = crear_archivo_pgm(filename, ancho_pixeles, alto_pixeles, arreglo_de_intensidades);
	if (res == -1){
		perror("Error al crear el archivo pgm");
		return -1;
	}

	free(arreglo_de_intensidades);
	destruir_complejo(centro);
	destruir_complejo(constante);
	return 0;
}
