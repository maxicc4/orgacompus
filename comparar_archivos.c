#include <stdio.h>
#include <stdlib.h>

int main(void){
	
	FILE* archivo_en_c;
	FILE* archivo_en_mips32;
    char* linea_c = NULL;
	char* linea_mips32 = NULL;
    size_t len_c = 0;
	size_t len_mips32 = 0;
	int valor_en_c, valor_en_mips32;

    archivo_en_c = fopen("ARCHIVO HECHO EN C", "r");
    if (!archivo_en_c)
        return -1;
	
	archivo_en_mips32 = fopen("ARCHIVO HECHO EN mips32", "r");
    if (!archivo_en_mips32)
        return -1;

    while (((read_c = getline(&linea_c, &len_c, archivo_en_c)) != -1) && ((read_mips32 = getline(&linea_mips32, &len_mips32, archivo_en_mips32)) != -1)){
        valor_en_c = atoi(linea_c);
		valor_en_mips32 = atoi(linea_mips32);
		if ((valor_en_mips32 < (valor_en_c - 5)) || (valor_en_mips32 > (valor_en_c + 5))){
			printf("Los archivos no son similares");
			return -1;
		}
	}

    fclose(archivo_en_c);
	fclose(archivo_en_mips32);
	
    if (linea_c)
        free(linea_c);
	if (linea_mips32)
        free(linea_mips32);
	
	printf("Los archivos son similares");
	
    return 0;
}
	