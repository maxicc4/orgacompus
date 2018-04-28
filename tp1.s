.text 
.align 2
.ent mips32_plot  #creo que esta linea es obligatoria
.globl mips32_plot		

mips32_plot:

	subu $sp , $sp ,32  	# reservamos stack para crear el stack frame
	sw $ra , 20($sp)	# salvamos la direccion de llama register adres en el stack frame
	sd a0 , 32($sp) 	# colocamos el registro que recibe por argumento en el stack frame 	

la t0, 16($fp)	# t1 <- &params    
	lw t1, 40(t0)	# t1 <- (params->shades)
	lw t2, 28(t0)	# t2 <- (params->s_re)
	lw t3, 32(t0)	# t3 <- (params->s_im) lo puse en save register porque el valor persiste durante la llamada
	lw f1, nnn	# f1 <- (param->UL_im)
	lw f2, nnn	# f2 <- (param-> UL_re)
	lw f3, nnn	# f3 <- (param->x_res)
	lw f4, nnn	# f4 <- (param->y_res)
	lw f5, nnn	# f5 <- (param-> d_re)
	lw f6, nnn	# f6 <- (param-> d_im)

	addi t1 , zero,0     				# y = 0
	move f1 , t2  					# coloca UL_im a t2 que contiene  ci = parms->UL_ima 
	
loop0:  blt t1, f4 , exit_loop0 			# pregunta si y < parms->y_res
	
	addi t1 , t1 , 1   				# incremento el contador del 1 for y en 1
	sub t2 , t2 , f6				# ci -= parms->d_im  estas dos lineas realizan los incrementos y decrementos del los 								# cont del for
	
	addi t2 , zero, 0 				# x = 0	
	move f2 , t3  					# coloca UL_re a t3 que contiene a cr  estas dos lineas setean contadores del for

loop1: blt t2 , f3 , exit_loop1				# x < parms->x_res

	addi t2 , t2 , 1				# ++y 			
	add t3 , t3 , f5				# cr += parms->d_re	
        move t3 , t4 					# zr = cr;   t4 = zr
	move t2 , t5					# zi = ci;   t5 = zi
	
	addi t4, zero, 0				# c = 0

loop:	beq t4, t1, exit_loop

	mul t5, (donde este zr), (donde este zr)	# t5 = zr*zr
	mul t6, (donde este zi), (donde este zi)	# t6 = zi*zi
	add t7, t5, t6					# t7 = zr*zr + zi*zi
	bgt t7, 4, exit_loop
	sub t8, t5, t6
	add t8, t2, t8					# t8 <- tr = (parms->s_re) + zr*zr - zi*zi
	mul t9, (donde este zr), (donde este zi)	# creo que t9 no se puede usar
	mul t9, t9, 2
	add t9, t3, t9					# t9 <- ti = (parms->s_im) + zr * zi * 2.0f
	move (donde este zr), t8			# zr = tr
	move (donde este zi), t9			# 
zi = ti
	addi t4, zero, 1				# c++

							# creo q aca deberia ir un jump to loop pero no estoy seguro 

exit_loop:     # aca sale del tercer for

j loop0							# salto de nuevo al loop0 para seguir iterando

exit_loop1:    # aca sale del segundo for  los contadores de los for los incremente mas arriba, debajo de sus respectivos branch de validacion

j loop1							# salto de nuevo al loop1 para seguir iterando

exit_loop0 :    #aca salio del primer for 
