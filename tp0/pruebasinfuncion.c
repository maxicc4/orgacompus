#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	int i=0;
	// pongo valores por defecto 
	int ancho = 640;
	int alto = 480;
	double h_rec = 2;
	double w_rec = 2;
	double center_x = 0;
	double center_y = 0;
	double seed_x =-0.726895347709114071439;
	double seed_y = 0.188887129043845954792;
 	char salida = 'c';
	char* archivo = NULL;

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
				   //printf("%s",str1);
				   // agarre la parte antes de la x
				   //ancho = atof(argv[i+1]);
				   const char separador = 'x';
				   char *valor2;
				   valor2 = strchr(argv[i+1], separador);
				  // printf("String afteRRRr |%s|\n", valor2+1);	

				   valor2 = valor2 +1;
				   //printf("%i",atoi(str1));
				   if(atoi(str1) > 0 && atoi(valor2) > 0)
				   {
					//printf("HOLA");
				   	ancho = atoi(str1);
					alto = 	atoi(valor2);
				   	printf("ancho:%i ",ancho );
					printf("alto:%i ",alto );
				   }
				 //  printf("First matched character is ATT %d\n", len + 1);
				   
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
					//printf("%s",argv[i+1]);
					if(strcspn (argv[i+1], "+") < strlen(argv[i+1]))
					{
						
						int longitud = strcspn (argv[i+1], "+");
					        strncat(valor1, argv[i+1]-1, longitud+1);
						printf("valor1::::%s:::",valor1);
						const char separador = '+';
				  		
				  		valor2 = strchr(argv[i+1], separador);
				  		//printf("String afteRRRr |%c| is - |%s|\n", separador, valor2+1);	
				  		//printf(str1);
				  		//valor2 = valor2 +1;
				  		printf("%i",atoi(valor2));
					}
					if(strcspn (argv[i+1], "-") < strlen(argv[i+1]))
					{
						//const char valor1[20];
						int longitud = strcspn (argv[i+1], "-");
					        strncat(valor1, argv[i+1]-1, longitud+1);
						//printf("%s",valor1);
						//printf("%s",valor1);
						const char separador = '-';
				  		//char *valor2;
				  		valor2 = strchr(argv[i+1], separador);
				  		//printf("String afteRRRr |%c| is - |%s|\n", separador, valor2+1);	
				  		//printf(str1);
				  		//valor2 = valor2 +1;
				  		//printf("%i",atoi(valor2));
					}
				}
                        	argv[i+1] = argv[i+1]-1; 
				if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--center"))
				{
					if( ( (atof(valor1) != 0) || (valor1[0] == '0') ) && ((atof(valor2) != 0) || (valor2[0] == '0')) )
					{
						printf("HOLAcen");
					   	center_x = atof(valor1);
						center_y = atof(valor2);
					   	printf(" centerx:%f",center_x );
						printf("centery:%f",center_y );
					}
				}
				if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--seed"))
				{
					if( ( (atof(valor1) != 0) || (valor1[0] == '0') ) && ((atof(valor2) != 0) || (valor2[0] == '0')) )
					{
						printf("HOLAseed");
					   	seed_x = atof(valor1);
						seed_y = atof(valor2);
					   	printf("seedx:%f",seed_x );
						printf("seedy:%f",seed_y );
					}
				}       
			}
		}
		
		if(!strcmp(argv[i], "-w") || !strcmp(argv[i], "--width"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				//double ttt;
				if( atof(argv[i+1]) > 1)
				{
					w_rec = atof(argv[i+1]);
				}
			}
		}
		if(!strcmp(argv[i], "-H") || !strcmp(argv[i], "--height"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				if(atof(argv[i+1]) > 1) // validamos q sea mayor a 1 quitamos la chance q sea negativo
				{
					h_rec = atof(argv[i+1]);
				}
				
			}
		}
		if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--seed"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				
				//printf(testvar); // se debe ver que es un punto valido viene numero+numeroi debe ser parceado
			}
		}
		if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
		{	
			printf("HOLA");
			const char separador = '.';
			char *valor2;
			valor2 = strchr(argv[i+1], separador);
			printf("%s",valor2);	
			if( (argv[i+1] != NULL) && (!strcmp(valor2, ".pgm" ) ) )// si existe el siguiente
			{	
				printf("HOLA1");
				archivo = argv[i+1];// debemos ver si es un archivo valido
				printf("%s",archivo);
			}
		}					
        }
	
	
				

//	printf("%s\n",argv[1]);
	return 0;
}
 
