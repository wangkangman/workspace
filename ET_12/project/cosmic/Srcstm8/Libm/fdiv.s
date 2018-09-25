;	FLOAT DIVIDE
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in FLOAT ACCU
;	- second operand pointed by X
;
	xdef	d_fdiv, d_okdiv
	xref.b	c_lreg, c_x
	.dcall	"9,0,d_fdiv"
;
d_fdiv:
	ld	a,c_lreg	; test zero
	or	a,c_lreg+1
	jreq	fini		; yes, exit
	ld	a,(3,x)		; copy second
	push	a		; on the stack
	ld	a,(2,x)
	push	a
	ld	a,(1,x)
	push	a
	ld	a,(x)
d_okdiv:
	push	a
	or	a,(2,sp)	; test zero
	jrne	panul		; not nul, continue
	addw	sp,#4		; clean stack
fini:
	retf			; and return
panul:
	pushw	y		; save register
	ld	a,(3,sp)	; load exponent
	btjf	c_lreg,#7,sok	; if sign set
	xor	a,#$80		; invert sign
	ld	(3,sp),a	; of result
sok:
	or	a,#$80		; force sign bit
	sll	(4,sp)		; extract bit
	rlc	a		; exponent ok and set carry
	ld	c_x,a		; store exponent
	rrc	(4,sp)		; align and set hidden bit
	ld	a,c_lreg+1
	bset	c_lreg+1,#7	; set hidden bit
	sll	a		; set carry with last bit
	ld	a,c_lreg	; get exponent
	rlc	a		; aligned
	sub	a,c_x		; substract exponents
	add	a,#127		; add offset
	ld	c_lreg,a	; store in place
	mov	c_x+1,#24	; count
	sra	(3,sp)		; prepare extension bit
	sll	(3,sp)		; but keep sign
	ld	a,c_lreg+1	; load MSB
	ldw	x,c_lreg+2	; load LSW
bcl:
	sllw	y		; shift result
	rlc	c_x
	sra	(3,sp)		; test extension
	jrc	gosub		; substract
	cp	a,(4,sp)	; compare values
	jrult	pasub		; less, continue
	jrugt	gosub		; high, substract
	cpw	x,(5,sp)
	jrult	pasub		; less, continue
gosub:
	subw	x,(5,sp)	; substract divisor
	sbc	a,(4,sp)
	incw	y		; set result bit
pasub:
	sllw	x		; shift dividend
	rlc	a
	rlc	(3,sp)		; keep carry
	dec	c_x+1		; count down
	jrne	bcl		; and loop back
	ld	a,c_x		; test MSB
	jrmi	panor		; normalize ok
	sllw	y		; shift resul
	rlc	a		; one bit enough
	dec	c_lreg		; update exponent
panor:
	sll	a		; remove hidden bit
	sll	(3,sp)		; get sign
	rrc	c_lreg		; set exponent
	rrc	a		; set mantissa
	ld	c_lreg+1,a	; in place
	ldw	c_lreg+2,y	; complete result
	popw	y		; restore register
	addw	sp,#4		; clean stack
	retf			; and return
;
	end
