#!/bin/sh

# Se controla que el archivo tenga la cantidad de lineas esperada
prueba1()
{
	RESULTADO="$(./tp1 -m mips32 | wc -l | sed 's/307204/OK/g')"
	if [ $RESULTADO != "OK" ]; then
		RESULTADO="ERROR"
	fi
}

# Se controla que el archivo tenga la cantidad de lineas esperada
prueba2()
{
	RESULTADO="$(./tp1 -m mips32 -r 1024x768 | wc -l | sed 's/786436/OK/g')"
	if [ $RESULTADO != "OK" ]; then
		RESULTADO="ERROR"
	fi
}

# Se verifica que la salida del programa no pueda pisar al ejecutable
prueba3()
{
	RESULTADO="$(./tp1 -m mips32 -o tp1 2>&1 | grep -q 'cannot open output file.' ; echo $? | sed 's/0/OK/g')"
	if [ $RESULTADO != "OK" ]; then
		RESULTADO="ERROR"
	fi
}

# Se verifica que se cree el archivo al pasarle un nombre de archivo
prueba4()
{
	file="prueba4.pgm"
	./tp1 -m mips32 -o $file
	RESULTADO="ERROR"
	if [ -f "$file" ]; then
		RESULTADO="OK"
		rm $file
	fi
}

prueba5()
{
	file1="prueba5_en_C.pgm"
	file2="prueba5_en_Mips32.pgm"
	./tp1 -o $file1
	./tp1 -m mips32 -o $file2
	./comparar $file1 $file2
}

prueba6()
{
	file1="prueba6_en_C.pgm"
	file2="prueba6_en_Mips32.pgm"
	./tp1 -c 0.282-0.007i -w 0.005 -H 0.005 -o $file1
	./tp1 -m mips32 -c 0.282-0.007i -w 0.005 -H 0.005 -o $file2
	./comparar $file1 $file2
}

prueba7()
{
	file1="prueba7_en_C.pgm"
	file2="prueba7_en_Mips32.pgm"
	./tp1 -s 0+0.8i -o $file1
	./tp1 -m mips32 -s 0+0.8i -o $file2
	./comparar $file1 $file2
}

prueba8()
{
	file1="prueba8_en_C.pgm"
	file2="prueba8_en_Mips32.pgm"
	./tp1 -s 0.8+0i -o $file1
	./tp1 -m mips32 -s 0.8+0i -o $file2
	./comparar $file1 $file2
}

prueba9()
{
	file1="prueba9_en_C.pgm"
	file2="prueba9_en_Mips32.pgm"
	./tp1 -r 200x200 -s 0.8+0i -w 0.01 -H 0.01 -o $file1
	./tp1 -m mips32 -r 200x200 -s 0.8+0i -w 0.01 -H 0.01 -o $file2
	./comparar $file1 $file2
}

prueba10()
{
	file1="prueba10_en_C.pgm"
	file2="prueba10_en_Mips32.pgm"
	./tp1 -r 400x300 -s 0.8+0i -c 0.282-0.007i -w 0.005 -H 0.005 -o $file1
	./tp1 -m mips32 -r 400x300 -s 0.8+0i -c 0.282-0.007i -w 0.005 -H 0.005 -o $file2
	./comparar $file1 $file2
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
echo "Prueba 5:"
prueba5
echo "Prueba 6:"
prueba6
echo "Prueba 7:"
prueba7
echo "Prueba 8:"
prueba8
echo "Prueba 9:"
prueba9
echo "Prueba 10:"
prueba10
