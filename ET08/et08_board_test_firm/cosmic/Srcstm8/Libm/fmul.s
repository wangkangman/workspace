;	FLOAT MULTIPLY
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in FLOAT ACCU
;	- second operand pointed by X
;
	xdef	d_fmul, d_okmul
	xref.b	c_lreg, c_x
	.dcall	"3,0,d_fmul"
;
d_fmul:
	ld	a,c_lreg	; test zero
	or	a,c_lreg+1
	jreq	fini		; yes, exit
	ld	a,(3,x)		; copy second
	push	a		; on the stack
	ld	a,(2,x)
	push	a
	ldw	x,(x)
	pushw	x
d_okmul:
	ldw	x,(1,sp)	; test zero
	jrne	panul		; not nul, continue
	ldw	c_lreg,x	; clear
	ldw	c_lreg+2,x	; result
	addw	sp,#4		; clean stack
fini:
	retf			; and return
panul:
	ld	a,(1,sp)	; load exponent
	btjf	c_lreg,#7,sok	; if sign set
	xor	a,#$80
	ld	(1,sp),a
sok:
	or	a,#$80		; force sign bit
	sll	(2,sp)		; set carry with last bit
	rlc	a		; exponent ok and set carry
	rrc	(2,sp)		; align and set hidden bit
	sub	a,#126		; remove offset
	ld	c_x,a		; store exponent
	ld	a,c_lreg+1
	bset	c_lreg+1,#7	; set hidden bit
	sll	a		; set carry with last bit
	ld	a,c_lreg	; get exponent
	rlc	a		; aligned
	add	a,c_x		; add exponents (with one offset)
	ld	c_lreg,a	; store in place
	ld	a,(2,sp)
	ldw	x,c_lreg
	mul	x,a		; H * H
	ldw	c_x,x		; keep MSW
	ld	a,(3,sp)
	ldw	x,c_lreg
	mul	x,a		; M * H
	clr	c_x+2		; prepare accu
	callr	niv2
	ld	a,(4,sp)
	ldw	x,c_lreg
	mul	x,a		; L * H
	ld	a,xl
	callr	niv3
	ld	a,(2,sp)
	ldw	x,c_lreg+1
	mul	x,a		; H * M
	callr	niv2
	ld	a,(3,sp)
	ldw	x,c_lreg+1
	mul	x,a		; M * M
	callr	niv3a
	ld	a,(4,sp)
	ldw	x,c_lreg+1
	mul	x,a		; L * M
	clr	c_lreg+2
	callr	niv4
	ld	a,(2,sp)
	ldw	x,c_lreg+2
	mul	x,a		; H * L
	callr	niv3a
	ld	a,(3,sp)
	ldw	x,c_lreg+2
	mul	x,a		; M * L
	callr	niv4
	ld	a,(4,sp)
	ldw	x,c_lreg+2
	mul	x,a		; L * L
	ld	a,xh
	add	a,c_lreg+2
	jrnc	noc2
	inc	c_lreg+1
	jrne	noc2
	callr	car3
noc2:
	ldw	x,c_x+1
	ld	a,c_x		; test MSB
	jrmi	padec		; normalize ok
	sll	c_lreg+1	; shift result
	rlcw	x		; left
	rlc	a
	dec	c_lreg		; update exponent
padec:
	sll	a		; remove hidden bit
	sll	(1,sp)		; extract sign
	rrc	c_lreg		; in place
	rrc	a		; align mantissa
	sll	c_lreg+1	; round up in carry
	ld	c_lreg+1,a	; in place
	ldw	c_lreg+2,x	; result
	jrnc	okr
	bset	c_lreg+3,#0	; simple round up
okr:
	addw	sp,#4		; clean stack
	retf			; and return
;
;	propagate carry upward
;
niv2:
	addw	x,c_x+1
	ldw	c_x+1,x
	jrnc	noc1
car1:
	inc	c_x
noc1:
	ret
;
niv3a:
	ld	a,xl
	add	a,c_lreg+1
niv3:
	ld	c_lreg+1,a
	ld	a,xh
	adc	a,c_x+2
	ld	c_x+2,a
	jrnc	noc1
car2:
	ldw	x,c_x
	incw	x
	ldw	c_x,x
	ret
;
niv4:
	addw	x,c_lreg+1
	ldw	c_lreg+1,x
	jrnc	noc1
car3:
	inc	c_x+2
	jreq	car2
	ret
;
	end
