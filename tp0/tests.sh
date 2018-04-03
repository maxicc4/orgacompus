#!/bin/sh

# Se controla que el archivo tenga la cantidad de lineas esperada
prueba1()
{
	RESULTADO="$(./tp0 | wc -l | sed 's/307204/OK/g')"
	if [ $RESULTADO != "OK" ]; then
		RESULTADO="ERROR"
	fi
}

# Se controla que el archivo tenga la cantidad de lineas esperada
prueba2()
{
	RESULTADO="$(./tp0 -r 1024x768 | wc -l | sed 's/786436/OK/g')"
	if [ $RESULTADO != "OK" ]; then
		RESULTADO="ERROR"
	fi
}

# Se verifica que la salida del programa no pueda pisar al ejecutable
prueba3()
{
	RESULTADO="$(./tp0 -o tp0 2>&1 | grep -q 'Error al abrir archivo' ; echo $? | sed 's/0/OK/g')"
	if [ $RESULTADO != "OK" ]; then
		RESULTADO="ERROR"
	fi
}

# Se verifica que se cree el archivo al pasarle un nombre de archivo
prueba4()
{
	file="prueba4.pgm"
	./tp0 -o $file
	RESULTADO="ERROR"
	if [ -f "$file" ]; then
		RESULTADO="OK"
		rm $file
	fi
}

echo "Ejecutando pruebas"

prueba1
echo "Prueba 1: $RESULTADO"
prueba2
echo "Prueba 2: $RESULTADO"
prueba3
echo "Prueba 3: $RESULTADO"
prueba4
echo "Prueba 4: $RESULTADO"