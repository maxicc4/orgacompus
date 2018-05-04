#include <mips/regdef.h>
#include <sys/syscall.h>

	.text			# segmento de texto del programa

	.abicalls
	.align 2		# alineacion 2^2

	.globl	mips32_plot
	.ent	mips32_plot
mips32_plot:
	.frame	$fp, 72, ra	# $fp: registro usado como frame pointer
				# 72: tamañodel stack frame
				# ra: registro que almacena el return address
	
	# bloque para codigo PIC
	.set	noreorder	# apaga reordenamiento de instrucciones
	.cpload t9		# directiva usada para codigo PIC
	.set	reorder		# enciende reordenamiento de instrucciones

	# se crea stack frame
	subu	sp, sp, 72	# 6 (ABA) + 12 (SRA: s0..s7,gp,fp,ra,offset)

	# directiva para codigo PIC
	.cprestore 56		# inserta aqui "sw gp, 56(sp)",
				# mas "lw gp, 56(sp)" luego de cada jal.
	# salvado de callee-saved regs en SRA
	sw	s0, 24(sp)
	sw	s1, 28(sp)
	sw	s2, 32(sp)
	sw	s3, 36(sp)
	sw	s4, 40(sp)
	sw	s5, 44(sp)
	sw	s6, 48(sp)
	sw	s7, 52(sp)
	sw	$fp, 60(sp)
	sw	ra, 64(sp)

	# de aqui al fin de la funcion uso $fp en lugar de sp.
	move	$fp, sp

	# salvo el argumento
	sw	a0, 72($fp)

	# Pongo el argumento en t0
	lw	t0, 72($fp)
	
	# Se imprime el header PGM en un solo fprintf
	# fprintf(parms->fp,"P2\n%u\n%u\n%u\n",parms->x_res,parms->y_res,parms->shades-1)
	lw	a0, 44(t0)		# a0 <- (parms->fp)
	la	a1, encabezado	# a1 <- ("P2\n%u\n%u\n%u\n")
	lw	a2, 32(t0)		# a2 <- (parms->x_res)
	lw	a3, 36(t0)		# a3 <- (parms->y_res)
	lw	t1, 40(t0)		# t1 <- (parms->shades)
	addiu t1, t1, -1	# t1 = t1 - 1
	sw	t1, 16($fp)		# guardo t1 en el quinto lugar de la ABA para que lo use fprintf
	jal	fprintf

	# CONTROLAR ERROR DE ESCRITURA



	l.s $f0, fp2	# f0 <- 2.0
	l.s $f1, fp4	# f1 <- 4.0

	lw t0, 72($fp)	# t0 <- &params
	l.s $f2, 0(t0)	# f2 <- (param->UL_re)
	l.s $f3, 4(t0)	# f3 <- (param->UL_im)
	l.s $f4, 16(t0)	# f4 <- (param->d_re)
	l.s $f5, 20(t0)	# f5 <- (param->d_im)
	l.s $f6, 24(t0)	# f6 <- (params->s_re)
	l.s $f7, 28(t0)	# f7 <- (params->s_im)
	lw s1, 32(t0)	# s1 <- (param->x_res)
	lw s2, 36(t0)	# s2 <- (param->y_res)
	lw s0, 40(t0)	# s0 <- (params->shades)
	

	addi s3 , zero, 0     				# y = 0
	mov.s $f8, $f3					# ci = (parms->UL_im) (F3 ESTA DISPONIBLE)
	
loop0:  bgeu s3, s2, exit_loop0 				# (y >= parms->y_res)
	
	addi s3, s3, 1   				# y++
	sub.s $f8, $f8, $f5				# (ci -= parms->d_im)

	addi s4 , zero, 0 				# x = 0
	mov.s $f9, $f2  				# cr = (parms->UL_re)

loop1:  bgeu s4, s1, exit_loop1				# (x >= parms->x_res)

	addi s4, s4, 1					# x++		
	add.s $f9, $f9, $f4				# (cr += parms->d_re)	

    mov.s $f10, $f9 				# zr = cr	OJO ORDEN!
	mov.s $f11, $f8					# zi = ci	OJO ORDEN! VS ACTUALIZAR CR Y CI
	
	addi s5, zero, 0				# c = 0

loop2:	bgeu s5, s0, exit_loop2
	
	mul.s $f12, $f10, $f10				# f12 = zr*zr
	mul.s $f13, $f11, $f11				# f13 = zi*zi
	add.s $f14, $f12, $f13 				# f14 = zr*zr + zi*zi
	c.lt.s $f14, $f1
	bc1f exit_loop2
	sub.s $f15, $f12, $f13				
	add.s $f15, $f15, $f6				# f15 <- tr = (parms->s_re) + zr*zr - zi*zi
	mul.s $f3, $f10, $f11				
	mul.s $f3, $f3, $f0
	add.s $f3, $f3, $f7				# f3 <- ti = (parms->s_im) + zr * zi * 2.0f
	mov.s $f10, $f15				# zr = tr
	mov.s $f11, $f3					# zi = ti
	addi s5, s5, 1					# c++
	j loop2

exit_loop2:     # aca sale del tercer for


	# Pongo el argumento en t0
	lw	t0, 72($fp)

	# Se imprimen las intensidades
	lw	a0, 44(t0)
	la	a1, intensidad
	move	a2, s5
	jal	fprintf				# OJO QUE AL LLAMAR A fprintf SE PUEDEN PERDER LOS VALORES DE ALGUNOS REGISTROS



	j loop1						# salto de nuevo al loop0 para seguir iterando
	

exit_loop1:    # aca sale del segundo for  los contadores de los for los incremente mas arriba, debajo de sus respectivos branch de validacion

	j loop0						# salto de nuevo al loop1 para seguir iterando

exit_loop0:    #aca salio del primer for 

	#ACA FALTA TRADUCIR LINEAS 59-63

	# return;
	# restauro callee-saved regs
	lw	s0, 24(sp)
	lw	s1, 28(sp)
	lw	s2, 32(sp)
	lw	s3, 36(sp)
	lw	s4, 40(sp)
	lw	s5, 44(sp)
	lw	s6, 48(sp)
	lw	s7, 52(sp)
	lw	gp, 56(sp)
	lw	$fp, 60(sp)
	lw	ra, 64(sp)
	# destruyo stack frame
	addu	sp, sp, 72
	# vuelvo a funcion llamante
	jr	ra

	.end	mips32_plot
	.size	mips32_plot,.-mips32_plot
	

	.rdata			# segmento read-only data
fp2:
	.float 2.0
fp4:
	.float 4.0
encabezado:
	.asciiz "P2\n%u\n%u\n%u\n"
intensidad:
	.asciiz "%u\n"
