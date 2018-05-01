#include <mips/regdef.h>
#include <sys/syscall.h>

	.text			# segmento de texto del programa

	.abicalls
	.align 2		# alineacion 2^2

	.globl	mips32_plot
	.ent	mips32_plot
mips32_plot:
	.frame	$fp, 40, ra	# $fp: registro usado como frame pointer
				# 40: tamañodel stack frame
				# ra: registro que almacena el return address
	
	# bloque para codigo PIC
	.set	noreorder	# apaga reordenamiento de instrucciones
	.cpload t9		# directiva usada para codigo PIC
	.set	reorder		# enciende reordenamiento de instrucciones

	# se crea stack frame
	subu	sp, sp, 40	# 6 (ABA) + 4 (SRA)

	# directiva para codigo PIC
	.cprestore 24		# inserta aqui "sw gp, 24(sp)",
				# mas "lw gp, 24(sp)" luego de cada jal.
	# salvado de callee-saved regs en SRA	
	sw	$fp, 28(sp)
	sw	ra, 32(sp)

	# de aqui al fin de la funcion uso $fp en lugar de sp.
	move	$fp, sp

	# salvo el argumento
	sw	a0, 40($fp)

	# Pongo el argumento en t0
	lw	t0, 40($fp)
	
	# Se imprime el header PGM en un solo printf
	# fprintf(parms->fp,"P2\n%u\n%u\n%u\n",parms->x_res,parms->y_res,parms->shades-1)
	lw	a0, 44(t0)		# a0 <- (parms->fp)
	la	a1, encabezado	# a1 <- ("P2\n%u\n%u\n%u\n")
	lw	a2, 32(t0)		# a2 <- (parms->x_res)
	lw	a3, 36(t0)		# a3 <- (parms->y_res)
	lw	t1, 40(t0)		# t1 <- (parms->shades)
	addiu t1, t1, -1	# t1 = t1 - 1
	sw	t1, 16($fp)		# guardo t1 en el quinto lugar de la ABA para que lo use fprintf
	jal	fprintf




	# return;
	# restauro callee-saved regs
	lw	gp, 24(sp)
	lw	$fp, 28(sp)
	lw	ra, 32(sp)
	# destruyo stack frame
	addu	sp, sp, 40
	# vuelvo a funcion llamante
	jr	ra

	.end	mips32_plot
	.size	mips32_plot,.-mips32_plot
	

	.rdata			# segmento read-only data
encabezado:
	.asciiz "P2\n%u\n%u\n%u\n"
