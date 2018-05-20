#include <mips/regdef.h>
#include <sys/syscall.h>

	.text

	.abicalls
	.align 2					# alineacion 2^2

	.globl	mips32_plot
	.ent	mips32_plot
	
mips32_plot:

	.frame	$fp, 128, ra			# $fp: registro usado como frame pointer
									# 128: tamano del stack frame
									# ra: registro que almacena el return address
	
	# Bloque para codigo PIC
	.set	noreorder				# Apaga reordenamiento de instrucciones
	.cpload t9					# Directiva usada para codigo PIC
	.set	reorder					# Enciende reordenamiento de instrucciones

	# Se crea stack frame
	subu	sp, sp, 128				# 4 (ABA) + 6 (LTA) + 10 (FRA) + 12 (SRA: s0..s7,gp,fp,ra,offset)

	# Directiva para codigo PIC
	.cprestore 112					# inserta aqui "sw gp, 112(sp)",
							# mas "lw gp, 112(sp)" luego de cada jal.
	# Salvado de callee-saved regs en SRA
	sw	s0, 80(sp)
	sw	s1, 84(sp)
	sw	s2, 88(sp)
	sw	s3, 92(sp)
	sw	s4, 96(sp)
	sw	s5, 100(sp)
	sw	s6, 104(sp)
	sw	s7, 108(sp)
	sw	$fp, 116(sp)
	sw	ra, 120(sp)

	# Se salvan los regs float en FRA
	s.s	$f20, 40(sp)
	s.s	$f22, 48(sp)
	s.s	$f24, 56(sp)
	s.s	$f26, 64(sp)
	s.s	$f28, 72(sp)

	# De aqui al fin de la funcion uso $fp en lugar de sp.
	move	$fp, sp

	# Salvo el argumento
	sw	a0, 128($fp)

	# Se imprime "P2\n"
	la	a1, encabezado			# a1 <- ("P2\n")
	li	a2, 3
	jal	_write
	bltz	v0, exit_error_io	# Control de error de escritura

	# Se imprime (parms->x_res)
	lw	t0, 128($fp)				# t0 <- (parms)
	lw	a0, 32(t0)				# a0 <- (parms->x_res)
	jal	_write_number
	bltz	v0, exit_error_io	# Control de error de escritura
	# Se imprime (parms->y_res)
	lw	t0, 128($fp)				# t0 <- (parms)
	lw	a0, 36(t0)				# a0 <- (parms->y_res)
	jal	_write_number
	bltz	v0, exit_error_io	# Control de error de escritura
	# Se imprime (parms->shades - 1)
	lw	t0, 128($fp)				# t0 <- (parms)
	lw	a0, 40(t0)				# a0 <- (parms->shades)
	addiu a0, a0, -1				# a0 = a0 - 1
	jal	_write_number
	bltz	v0, exit_error_io	# Control de error de escritura


	l.s $f0, fp2		# f0 <- 2.0
	l.s $f2, fp4		# f2 <- 4.0

	lw t0, 128($fp)		# t0 <- &params
	l.s $f4, 0(t0)		# f4 <- (param->UL_re)
	l.s $f6, 4(t0)		# f6 <- (param->UL_im)
	l.s $f8, 16(t0)		# f8 <- (param->d_re)
	l.s $f10, 20(t0)	# f10 <- (param->d_im)
	l.s $f12, 24(t0)	# f12 <- (params->s_re)
	l.s $f14, 28(t0)	# f14 <- (params->s_im)
	lw s1, 32(t0)		# s1 <- (param->x_res)
	lw s2, 36(t0)		# s2 <- (param->y_res)
	lw s0, 40(t0)		# s0 <- (params->shades)
	

	addi s3 , zero, 0     				# y = 0
	mov.s $f16, $f6					# ci = (parms->UL_im) (f6 esta disponible)
	
loop0:  bgeu s3, s2, exit_loop0 			# (y >= parms->y_res)
	
	addi s3, s3, 1   				# y++

	addi s4 , zero, 0 				# x = 0
	mov.s $f18, $f4  				# cr = (parms->UL_re)

loop1:  bgeu s4, s1, exit_loop1				# (x >= parms->x_res)

	addi s4, s4, 1					# x++
	
	mov.s $f20, $f18 				# zr = cr
	mov.s $f22, $f16				# zi = ci
	
	addi s5, zero, 0				# c = 0

loop2:	bgeu s5, s0, exit_loop2
	
	mul.s $f24, $f20, $f20				# f24 = zr*zr
	mul.s $f26, $f22, $f22				# f26 = zi*zi
	add.s $f28, $f24, $f26 				# f28 = zr*zr + zi*zi
	c.lt.s $f28, $f2
	bc1f exit_loop2
	sub.s $f28, $f24, $f26				
	add.s $f28, $f28, $f12				# f28 <- tr = (parms->s_re) + zr*zr - zi*zi
	mul.s $f6, $f20, $f22				
	mul.s $f6, $f6, $f0
	add.s $f6, $f6, $f14				# f6 <- ti = (parms->s_im) + zr * zi * 2.0f
	mov.s $f20, $f28				# zr = tr
	mov.s $f22, $f6					# zi = ti
	addi s5, s5, 1					# c++
	j loop2

exit_loop2:

	# Se imprime la intensidad
	move	a0, s5
	jal	_write_number
	bltz	v0, exit_error_io	# Control de error de escritura

	add.s $f18, $f18, $f8				# (cr += parms->d_re)
	j loop1
	

exit_loop1:

	sub.s $f16, $f16, $f10				# (ci -= parms->d_im)
	j loop0

exit_error_io:

	# write(2, "i/o error.\n", 11);
	# 2 es el file descriptor de stderr
	li	a0, 2
	la	a1, errorio
	li	a2, 11
	li	v0, SYS_write
	syscall

	# exit(1);
	li	a0, 1
	li	v0, SYS_exit
	syscall

exit_loop0: 

	# Pongo el argumento en t0
	lw	t0, 128($fp)

	lw	a0, 44(t0)
	jal	fflush
	bnez	v0, exit_cannotflush			# if (fflush(parms->fp) != 0)
	
	j exit_withouterror	

exit_cannotflush:
	
	# write(2, "cannot flush output file.\n", 26);
	# 2 es el file descriptor de stderr
	li	a0, 2
	la	a1, cannotflush
	li	a2, 26
	li	v0, SYS_write
	syscall

	# exit(1);
	li	a0, 1
	li	v0, SYS_exit
	syscall

exit_withouterror:

	# return;
	# Restauro callee-saved regs
	lw	s0, 80(sp)
	lw	s1, 84(sp)
	lw	s2, 88(sp)
	lw	s3, 92(sp)
	lw	s4, 96(sp)
	lw	s5, 100(sp)
	lw	s6, 104(sp)
	lw	s7, 108(sp)
	lw	gp, 112(sp)
	lw	$fp, 116(sp)
	lw	ra, 120(sp)

	# Se restauran los regs float
	l.s	$f20, 40(sp)
	l.s	$f22, 48(sp)
	l.s	$f24, 56(sp)
	l.s	$f26, 64(sp)
	l.s	$f28, 72(sp)

	# Destruyo stack frame
	addu	sp, sp, 128
	# Vuelvo a funcion llamante
	jr	ra

# FIN PROGRAMA PRINCIPAL


# Recibe como parametro un entero en a0
# Como maximo el numero puede tener 4 digitos (depende del tamanio reservado para LTA)
_write_number:
	
	sw	ra, 16($fp)				# Salvo el ra de _write_number en LTA
	move	t0, a0
	
	addiu	s6, $fp, 16			# Para guardar en el LTA desde la dir 20 (en la sig inst se le suma 4)
other_digit:
	addiu	s6, s6, 4
	remu	t1, t0, 10			# Resto division
	sll	t1, t1, 2				# Multiplico por 4 para acceder a la pos en el array (cada elem ocupa 4 bytes)
	sw	t1, 0(s6)
	divu	t0, t0, 10			# Division
	bnez	t0, other_digit		# Si la division es cero

print_digit:
	lw	t1, 0(s6)
	lw	a1, number(t1)
	li	a2, 1
	jal	_write

	addiu s6, s6, -4
	addiu	t1, $fp, 16
	bgt	s6, t1, print_digit		# Si hay mas digitos en el stack

#print_end_line
	la	a1, end_line
	li	a2, 1
	jal	_write

	lw	ra, 16($fp)
	jr	ra

# FIN _write_number

# Llama al syscall write
# Imprime en params->fp
# En a1 se especifica el puntero al buffer que va a ser escrito en el archivo
# En a2 la cantidad de bytes que van a ser escritos
_write:
	
	sw	ra, 36($fp)				# Salvo el ra de _write en LTA
	lw	t0, 128($fp)				# t0 <- (parms)
	lw	t1, 44(t0)				# t1 <- (parms->fp)
	# Se obtiene el file descriptor a traves del file pointer
	lhu	a0, 14(t1)				# a0 <- (fp->_file)
	li	v0, SYS_write
	syscall

	lw	ra, 36($fp)
	jr	ra

# FIN _write

	.end	mips32_plot
	.size	mips32_plot,.-mips32_plot
	

	.rdata						# segmento read-only data
fp2:
	.float 2.0
fp4:
	.float 4.0
encabezado:
	.asciiz "P2\n"
intensidad:
	.asciiz "%u\n"
errorio:
	.asciiz "i/o error.\n"
cannotflush:
	.asciiz "cannot flush output file.\n"

	.align 2
number:	.word number_0, number_1, number_2, number_3, number_4, \
	      number_5, number_6, number_7, number_8, number_9
	.size number, 40

	.align 0

number_0: .asciiz "0"
number_1: .asciiz "1"
number_2: .asciiz "2"
number_3: .asciiz "3"
number_4: .asciiz "4"
number_5: .asciiz "5"
number_6: .asciiz "6"
number_7: .asciiz "7"
number_8: .asciiz "8"
number_9: .asciiz "9"

end_line: .ascii "\n"
