#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
	
	FILE* archivo_en_c;
	FILE* archivo_en_mips32;
	char* linea_c = NULL;
	char* linea_mips32 = NULL;
	size_t len_c = 0;
	size_t len_mips32 = 0;
	int valor_en_c, valor_en_mips32;
	long pixeles_similares = 0;
	long pixeles_totales = 0;
	ssize_t read_c, read_mips32;

	archivo_en_c = fopen(argv[1], "r");
	if (!archivo_en_c)
		return -1;
	
	archivo_en_mips32 = fopen(argv[2], "r");
	if (!archivo_en_mips32)
		return -1;
	
	for (int i=0; i<4; i++){
		read_c = getline(&linea_c, &len_c, archivo_en_c);
		read_mips32 = getline(&linea_mips32, &len_mips32, archivo_en_mips32);
	}

	while (((read_c = getline(&linea_c, &len_c, archivo_en_c)) != -1) && ((read_mips32 = getline(&linea_mips32, &len_mips32, archivo_en_mips32)) != -1)){
		valor_en_c = atoi(linea_c);
		valor_en_mips32 = atoi(linea_mips32);
		if ((valor_en_c - 5) <= valor_en_mips32 && valor_en_mips32 <= (valor_en_c + 5))
			pixeles_similares++;
		pixeles_totales++;
	}

	fclose(archivo_en_c);
	fclose(archivo_en_mips32);
	
	if (linea_c)
		free(linea_c);
	if (linea_mips32)
		free(linea_mips32);

	float porcentaje = ((float)pixeles_similares/(float)pixeles_totales)*100;
	
	printf("Los archivos son %.2f%% por ciento similares\n", porcentaje);
	
	return 0;
}
	
