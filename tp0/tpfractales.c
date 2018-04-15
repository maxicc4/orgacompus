#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct{
	double parte_real;
	double parte_imaginaria;
}complejo_t;

/*Recibe dos numeros double y devuelve un complejo con dichos valores como parte real e imaginaria*/
complejo_t crear_complejo(double parte_real, double parte_imaginaria){
	complejo_t complejo;
	complejo.parte_real = parte_real;
	complejo.parte_imaginaria = parte_imaginaria;
	return complejo;
}

/*Recibe un complejo y devuelve su cuadrado como nuevo complejo*/
complejo_t elevar_complejo_al_cuadrado(complejo_t complejo){
	double parte_real = complejo.parte_real;
	double parte_imaginaria = complejo.parte_imaginaria;
	double parte_real_cuadrado = ((parte_real*parte_real)-(parte_imaginaria*parte_imaginaria));
	double parte_imaginaria_cuadrado = (2*parte_real*parte_imaginaria);
	complejo_t cuadrado = crear_complejo(parte_real_cuadrado,parte_imaginaria_cuadrado);
	return cuadrado;
}

/*Recibe dos complejos y devuelve su suma como nuevo complejo*/
complejo_t sumar_dos_complejos(complejo_t complejo1,complejo_t complejo2){
	double parte_real_suma = (complejo1.parte_real)+(complejo2.parte_real);
	double parte_imaginaria_suma = (complejo1.parte_imaginaria)+(complejo2.parte_imaginaria);
	complejo_t suma = crear_complejo(parte_real_suma,parte_imaginaria_suma);
	return suma;
}

/*Recibe un complejo y devuelve su modulo como double*/
double calcular_modulo(complejo_t complejo){
	double parte_real = complejo.parte_real;
	double parte_imaginaria = complejo.parte_imaginaria;
	return sqrt((parte_real*parte_real)+(parte_imaginaria*parte_imaginaria));
}

/*Recibe dos complejos donde el primero representa a un pixel y el segundo a la seed
y devuelve la siguiente iteracion del pixel segun la formula*/
complejo_t calcular_siguiente_iteracion(complejo_t complejo, complejo_t constante){
	complejo_t cuadrado = elevar_complejo_al_cuadrado(complejo);
	complejo_t suma = sumar_dos_complejos(cuadrado,constante);
	return suma;
}

/*Recibe un arreglo de enteros y todos los parametros necesarios para iterar
sobre cada pixel y guardar la cantidad de iteraciones que se realizaron sobre cada uno.*/
void calcular_intensidades(int* arreglo_de_intensidades, int ancho_pixeles, int alto_pixeles,
			double ancho_complejos, double alto_complejos, complejo_t constante, complejo_t centro){
	
	double altura_maxima = centro.parte_imaginaria + alto_complejos/2;
	double ancho_minimo = centro.parte_real - ancho_complejos/2;
	double step_altura = alto_complejos / alto_pixeles;
	double step_ancho = ancho_complejos / ancho_pixeles;

	complejo_t pixel;
	complejo_t pixelNuevo;
	int indice = 0;
	for (double j=0; j<alto_pixeles; j++){
		for (double i=0; i<ancho_pixeles; i++){
			pixel = crear_complejo(ancho_minimo + i*step_ancho,altura_maxima - j*step_altura);
			int cantidad_iteraciones = 0;
			while ((calcular_modulo(pixel)<=2)&&(cantidad_iteraciones<255)){
				pixelNuevo = calcular_siguiente_iteracion(pixel,constante);
				pixel = pixelNuevo;
				cantidad_iteraciones++;
			}
			arreglo_de_intensidades[indice] = cantidad_iteraciones;
			indice++;
		}
	}
	return;
}

/*Recibe un nombre de archivo, ancho, alto y las intensidades.
Crea un archivo pgm con la informacion necesaria para visualizar el fractal
Devuelve -1 en caso de error, 0 si no*/
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

void caso_r(int* anchox,int* altoy,char* optarg){
    if( strcspn (optarg, "x") > 0 && strcspn (optarg, "x") < strlen(optarg) ){	//entra sihay almenos 1 x y no es al inicio
	 char str1[20];
	 memset(&str1[0], 0, sizeof(str1));
	 int longitud = strcspn (optarg, "x");
	 strncat(str1, optarg, longitud);
	 const char separador = 'x';
	 char *valor2;
	 valor2 = strchr(optarg, separador);
	 valor2 = valor2 +1;
	 if(atoi(str1) > 40 && atoi(valor2) > 40){
	   	*anchox = atoi(str1);
		*altoy = atoi(valor2);
         }
	 else{
		 fprintf(stderr, "Error al leer resolucion :");
		 exit(EXIT_FAILURE);
	}
    }
    else{
    	fprintf(stderr, "Error al leer resolucion : ");
	exit(EXIT_FAILURE);
    }
}

void caso_complejo(double* center_x,double* center_y, char* optarg){
        char valor1[20];
        char *valor2;
	optarg = optarg + 1;
	memset(&valor1[0], 0, sizeof(valor1));
	if(  strcspn(optarg, "+") < strlen(optarg)  
		|| ( strcspn (optarg, "-") < strlen(optarg) ) ){//entra sihay almenos 1 + o un - ,y no es al inicio
			if(strcspn (optarg, "+") < strlen(optarg)){
				int longitud = strcspn (optarg, "+");
			        strncat(valor1, optarg-1, longitud+1);
				const char separador = '+';
		  		valor2 = strchr(optarg, separador);
		  	}
			if(strcspn (optarg, "-") < strlen(optarg)){
				//const char valor1[20];
				//printf("entro");				
				int longitud = strcspn (optarg, "-");
			        strncat(valor1, optarg-1, longitud+1);
				const char separador = '-';
		  		valor2 = strchr(optarg, separador);
			}
		}
		else{
			fprintf(stderr, "Error al leer  : numero complejo");
		 exit(EXIT_FAILURE);}
                optarg = optarg-1; 
		if( ( (atof(valor1) != 0) || (valor1[0] == '0') ) && ((atof(valor2) != 0) || (valor2[0] == '0'))){					
			
			if(valor2[strlen(valor2)-1]== 'i'){
				*center_x = atof(valor1);
				*center_y = atof(valor2);
			}
			else {
				fprintf(stderr, "Error al leer numero complejo");
				 exit(EXIT_FAILURE);}
		}
		else {
		fprintf(stderr, "Error al leer numero complejo");
		 exit(EXIT_FAILURE);
		}
}


/*Realiza la lectura de los argumentros pasados por consola y todas las validaciones necesarias
Devuelve '-' salida por consola, o el nombre del archivo destino*/
const char* lecturaArgumentos(int argc, char *argv[],int* anchox,int* altoy,double* h_rec,double* w_rec,double* center_x,double* center_y,
	double* seed_x,double* seed_y,const char* archivo){
	// pongo valores por defecto 
	int ancho = 640;
	int alto = 480;
	*center_x = 0;	*center_y = 0;	*seed_x =-0.726895347709114071439;*seed_y = 0.188887129043845954792;
 	archivo = "c";	*anchox = ancho; *altoy = alto; archivo = "-"; int c;
	bool entro[6]= {false , false , false , false , false , false};
        int digit_optind = 0;

   while (1) {
//        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
	int this_option_optind = optind ? optind : 1;
        
        static struct option long_options[] = {
           {"resolution",     required_argument, 0,'r'},
           {"center",     required_argument, 0,'c'},	
           {"width",  required_argument, 0, 'w'},
           {"height",  required_argument, 0, 'H'},
           {"seed",  required_argument, 0, 's'},
           {"output",  required_argument, 0, 'o'},
           {0,         0,                 0,  0 }
        };

       c = getopt_long(argc, argv, "r:c:w:H:s:o:02",
                 long_options, &option_index);
        if (c == -1){
   
	  break;
	}
       switch (c) {

	case '2':
            if (digit_optind != 0 && digit_optind != this_option_optind)
             // printf("digits occur in two different argv-elements.\n");
            digit_optind = this_option_optind;
            //printf("option %c\n", c);
	    return "fin";
            break;

       case 'r':
	    if(entro[0] == false )	
       	    	caso_r(anchox,altoy, optarg);
	    else{fprintf(stderr, "Error argumento repetido");
		 exit(EXIT_FAILURE);}
	    entro[0] = true;
	    break;

       case 'c':
	    
	     if(entro[1] == false )	
		caso_complejo(center_x,center_y, optarg);
	    else{fprintf(stderr, "Error argumento repetido");
		 exit(EXIT_FAILURE);}
	    entro[1] = true;
	    break;

       case 'w':
          if(entro[2] == false){ 
	  	if( atof(optarg) >= 0.00001)
	  	  {
          	  	*w_rec = atof(optarg);
       	  	  }
	  	  else {
			fprintf(stderr, "Error al leer ancho");
			exit(EXIT_FAILURE);
	  	  }
	  }
	  else{fprintf(stderr, "Error argumento repetido");
		 exit(EXIT_FAILURE);}
	  entro[2] = true;
            break;

       case 'H':
          if(entro[3] == false){ 
	  	if( atof(optarg) >= 0.00001)
	  	  {
          	  	*h_rec = atof(optarg);
       	  	  }
	  	  else {
			fprintf(stderr, "Error al leer alto");
			exit(EXIT_FAILURE);
	  	  }
	  }
	  else{fprintf(stderr, "Error argumento repetido");
		 exit(EXIT_FAILURE);}
	  entro[3] = true;
            break;
       case 's':
            if(entro[4] == false )	
       	    	caso_complejo(seed_x,seed_y, optarg);
	    else{fprintf(stderr, "Error argumento repetido");
		 exit(EXIT_FAILURE);}
	    entro[4] = true;
	    break;


       case 'o':
         if(entro[5] == false){
	    if(  (strcmp(optarg, "-" ) ) )// si no ingresamos salida por consola
	    {	
	    	archivo = optarg;// nombre que l epondremos al archivo
	    }
	 }else{fprintf(stderr, "Error argumento repetido");
		 exit(EXIT_FAILURE);}
	  entro[5] = true;
	    break;


    case '?':
	fprintf(stderr, "%s: opcion `-%c' es invalido programa terminado\n",argv[0], optopt);
	exit(EXIT_FAILURE);
        break;
        }
    }
	
   if (optind < argc) {
	//aca sale si entramos algo q no es opcion
        while (optind < argc)
        	printf("%s ", argv[optind++]);
        printf("\n");
	fprintf(stderr, "Error argumentos no validos ");
	exit(EXIT_FAILURE);
    }
	return archivo;
}


/*Funcion principal. Toma los argumentos recibidos por consola, o aquellos por defecto, realiza
 iteraciones para cada pixel y guarda la informacion necesaria en un archivo pgm para visualizar el fractal*/
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
	
	filename = lecturaArgumentos(argc,argv,&ancho_pixeles,&alto_pixeles,&ancho_complejos,&alto_complejos,
	&center_x,&center_y,&seed_x,&seed_y,filename);	
	
	
	
	complejo_t centro = crear_complejo(center_x,center_y);
	complejo_t constante = crear_complejo(seed_x,seed_y);
	
	int* arreglo_de_intensidades = malloc(ancho_pixeles*alto_pixeles*sizeof(int));
	if (!arreglo_de_intensidades){
		perror("Error al reservar memoria para la intensidad de pixeles");
		exit(EXIT_FAILURE);
	}

	calcular_intensidades(arreglo_de_intensidades, ancho_pixeles, alto_pixeles,
				 ancho_complejos, alto_complejos, constante, centro);
	
	int res = 0;
	res = crear_archivo_pgm(filename, ancho_pixeles, alto_pixeles, arreglo_de_intensidades);
	if (res == -1){
		perror("Error al crear el archivo pgm");
		free(arreglo_de_intensidades);
		exit(EXIT_FAILURE);
	}

	free(arreglo_de_intensidades);
	return 0;
}
