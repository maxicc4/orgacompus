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
	//printf("\ncmdline args count=%s", argc);

	 /* First argument is executable name only */
	//printf("\nexe name=%s", argv[0]);
	//char str[80] = "This is - www.tutorialspoint.com - website";
   	//const char s[2] = "-";
   	//char *token;
   
   /* get the first token */
	//token = strtok(str, s); 
	//printf( " %s\n", token );
//	char* substr; 
//	strncpy(substr, token, 2);

	for (i=1; i< argc; i++) // vamos recorriendo los argumentos
	{
		//printf("\narg%d=%s", i, argv[i]); muestra cada argumento
		//printf("entro");
		
		//printf(i);
		if(!strcmp(argv[i], "-r") || !strcmp(argv[i], "--resolution"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				if( strcspn (argv[i+1], "x") > 0 && strcspn (argv[i+1], "x") < strlen(argv[i+1]) )	 					//entra sihay almenos 1 x y no es al inicio
				{
				   const char str1[20];
				   int longitud = strcspn (argv[i+1], "x");
				   strncat(str1, argv[i+1], longitud);
				   //printf("%s",str1);
				   // agarre la parte antes de la x
				   //ancho = atof(argv[i+1]);
				   const char separador = 'x';
				   char *valor2;
				   valor2 = strchr(argv[i+1], separador);
				   //printf("String afteRRRr |%c| is - |%s|\n", separador, valor2+1);	
				   //printf(str1);
				   valor2 = valor2 +1;
				   printf("%i",atoi(str1));
				   if((double)atof(str1) > 0 && atof(valor2) > 0)
				   {
					printf("HOLLA");
				   	ancho = atoi(str1);
					alto = 	atoi(valor2);
				   	printf("%i",ancho );printf("%i",alto );
				   }
				   int len;
				   
				   const char str2[20] = "j";

				   len = strcspn(str1, str2);
				   
				   printf("First matched character is ATT %d\n", len + 1);
				   
				}


				

				//double ttt;
				//ttt=atof(argv[i+1]);
				//printf("%f\n", argv[i+1]);
				char testvar = argv[i+1][1];
				char src[50], dest[50];

				// strcpy(src,  "This is source");
				strcpy(dest, argv[i+1]+2);

				//strncat(dest, src, 15);

				printf("Final destination string : |%s|", dest);				
				printf("%c",testvar); // se parcea ingreso en formato numeroxnumero
				 
					const char str[] = "http://www.tutorixalspoint.com";
				   const char ch = 'x';
				   char *ret;

				   ret = strchr(str, ch);

				   printf("String after |%c| is - |%s|\n", ch, ret);
				       int len;
					   const char str1[] = "ABCDEFG019874";
					   const char str2[] = "BCDE";

					   len = strspn(str1, str2);

					   printf("Length of initial segment matching %d\n", len );
					    const char haystack[20] = "0.123x-1231";
				   const char needle[10] = "x";
				   //char *ret;

				   ret = strstr(haystack, needle);

				   printf("The substring is: %s\n", ret);
				  
				   //char src[50], dest[50];

				   strcpy(src,  "This is source");
				   strcpy(dest, "This is destination");

				   strncat(dest, src, 11);

				   printf("Final destination string : |%s|", dest);
   
				   
			}
		}
		if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--center"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				// se debe ver que es un punto valido viene numero+numeroi debe ser parceado
			}
		}
		if(!strcmp(argv[i], "-w") || !strcmp(argv[i], "--width"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				//double ttt;
				if(argv[i+1] > 1)
				{
					w_rec = atof(argv[i+1]);
				}
			}
		}
		if(!strcmp(argv[i], "-H") || !strcmp(argv[i], "--height"))
		{	
			if(argv[i+1] != NULL)// si existe el siguiente
			{	
				if(argv[i+1] > 1) // validamos q sea mayor a 1 quitamos la chance q sea negativo
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
			if(argv[i+1] != NULL && strcmp(argv[i], "-"))// si existe el siguiente
			{	
				archivo = argv[i+1];// debemos ver si es un archivo valido
			}
		}					
        }
	
	
				

//	printf("%s\n",argv[1]);
	return 0;
}
 
