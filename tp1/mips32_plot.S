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
	subu	sp, sp, 40

	# directiva para codigo PIC
	.cprestore 24		# inserta aqui "sw gp, 24(sp)",
				# mas "lw gp, 24(sp)" luego de cada jal.
	# salvado de callee-saved regs en SRA	
	sw	$fp, 28(sp)
	sw	ra, 32(sp)

	# de aqui al fin de la funcion uso $fp en lugar de sp.
	move	$fp, sp

	# salvo el primer argumento
	sw	a0, 40($fp)
	
	# fprintf(parms->fp,"P2\n# Trabajo practico de Organizacion de Computadoras\n%d %d\n255\n",640,480)
	lw	a0, 44(a0)
	la	a1, encabezado
	li	a2, 640
	li	a3, 480
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
	.asciiz "P2\n# Trabajo practico de Organizacion de Computadoras\n%d %d\n255\n"
