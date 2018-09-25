;	FLOAT ADDITION/SUBSTRACT
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in float accumulator
;	- second operand pointed by X
;
	xdef	d_fadd, d_fsub
	xdef	d_okadd, d_oksub
	xref.b	c_lreg, c_x
	.dcall	"9,0,d_fadd"
	.dcall	"9,0,d_fsub"
;
;	float substract
;
d_fsub:
	ld	a,(3,x)		; copy argument
	push	a		; on the stack
	ld	a,(2,x)
	push	a
	ld	a,(1,x)
	push	a
	ld	a,(x)
d_oksub:
	jrne	ok		; test zero
	tnz	(1,sp)		; if not nul
	jreq	d_okadd
ok:
	xor	a,#$80		; invert sign
	jra	d_okadd		; and continue
;
;	float addition
;
d_fadd:
	ld	a,(3,x)		; copy argument
	push	a		; on the stack
	ld	a,(2,x)
	push	a
	ld	a,(1,x)
	push	a
	ld	a,(x)
d_okadd:
	push	a		; test second operand
	or	a,(2,sp)	; against zero
	jreq	fini		; null, exit
	ldw	x,c_lreg	; test first operand
	jrne	panul		; not nul, constinue
recop:
	popw	x		; copy second
	ldw	c_lreg,x	; operand
	popw	x		; to
	ldw	c_lreg+2,x	; result
	retf			; and return
fini:
	addw	sp,#4		; clean stack
	retf			; and return
panul:
	pushw	y		; save register
	clr	c_x+1		; sign flag
	sllw	x		; exponent
	rlc	c_x+1		; keep sign
	ld	a,xh		; get exponent
	ld	c_x,a		; store first exponent
	ldw	x,(3,sp)	; extract
	sllw	x		; exponent
	rlc	c_x+1		; keep sign
	ld	a,xh		; get exponent
	clrw	x		; prepare count
	sub	a,c_x		; substract exponents
	jreq	padec		; nul, no shift
	ld	xl,a		; count in register
	jrugt	posit		; positive, shift first operand
	cp	a,#-23		; test underflow
	jrult	xfini		; 2nd nul, exit
	neg	a
	ld	xl,a		; positive count
	ldw	y,(5,sp)	; load LSW
	ld	a,(4,sp)	; load MSB
	or	a,#$80		; set hidden bit
bcl1:
	srl	a		; shift right
	rrcw	y
	decw	x		; count up
	jrne	bcl1		; and loop back
	ld	(4,sp),a	; store result
	ldw	(5,sp),y
	bset	c_lreg+1,#7	; set hidden bit on first operand
	jra	oper		; and operate
padec:
	bset	c_lreg+1,#7	; set hidden bit on first operand
	jra	oper2		; and continue
xfini:
	jra	cfini		; far jump
xrecop:
	popw	y		; clean-up stack
	jra	recop		; and copy result
posit:
	cp	a,#24		; test underflow
	jruge	xrecop		; yes, copy second operand
	add	a,c_x		; restore
	ld	c_x,a		; result exponent
	ldw	y,c_lreg+2	; load LSW
	ld	a,c_lreg+1	; load MSB
	or	a,#$80		; set hidden bit
bcl2:
	srl	a		; shift right
	rrcw	y
	decw	x		; count down
	jrne	bcl2		; and loop back
	ld	c_lreg+1,a	; store result
	ldw	c_lreg+2,y
oper2:
	ld	a,(4,sp)	; set hidden bit on second operand
	or	a,#$80
	ld	(4,sp),a
oper:
	ldw	x,c_lreg+2	; operand on 24 bits
	ld	a,c_lreg+1
	srl	c_x+1		; test signs
	jrugt	soust		; bits 10
	jrnc	addit		; bits 00
	jrne	addit		; bits 11
soust:
	subw	x,(5,sp)
	sbc	a,(4,sp)
	jrnc	rpos		; if carry
	cpl	c_x+1		; invert result sign
	cpl	a		; and invert result
	negw	x
	jrne	rpos
	inc	a
rpos:
	tnz	a		; test zero
	jrmi	panorm		; result ok
	jrne	norm		; not nul, shift
	tnzw	x		; continue test zero
	jrne	norm
	ldw	c_lreg+2,x	; result is zero
	ldw	c_lreg,x	; complete result
	jra	cfini		; and return
addit:
	addw	x,(5,sp)
	adc	a,(4,sp)
	jrnc	panorm		; if carry
	rrc	a		; shift right
	rrcw	x		; result
	inc	c_x		; update exponent
	jra	panorm		; and continue
norm:
	dec	c_x		; update exponent
	sllw	x		; shift right
	rlc	a
	jrpl	norm		; and loop again
panorm:
	ldw	c_lreg+2,x	; MSW in place
	sll	a		; remove hidden bit
	ld	xl,a
	ld	a,c_x		; exponent
	ld	xh,a
	srl	c_x+1		; sign of result
	rrcw	x		; in place
	ldw	c_lreg,x	; result completed
cfini:
	popw	y		; restore register
	addw	sp,#4		; clean stack
	retf			; and return
;
	end
