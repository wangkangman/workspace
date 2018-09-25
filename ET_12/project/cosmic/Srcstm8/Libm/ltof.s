;	LONG INTEGER TO FLOAT CONVERSION
;	Copyright (c) 2006 by COSMIC Software
;	- operand and result in FLOAT ACCU
;
	xdef	d_ltof
	xref.b	c_lreg
	.dcall	"7,0,d_ltof"
;
d_ltof:
	pushw	x		; save
	pushw	y		; registers
	ld	a,#23		; start exponent
	tnz	c_lreg		; check MSB
	jrne	panul		; not nul, continue
left:
	ldw	x,c_lreg+1	; result MSW
	jrmi	panor		; normalized, ok
	jrne	bcla		; test for zero
	tnz	c_lreg+3
	jreq	fini		; zero, exit
	ldw	x,c_lreg+2	; fast shift
	clr	c_lreg+3
	sub	a,#16		; update exponent
	swapw	x		; align and check if
	jrmi	panor		; already normalized
bcla:
	dec	a		; update exponent
	sll	c_lreg+3	; shift value
	rlcw	x
	jrpl	bcla		; and loop again
	jra	panor		; and continue
panul:
	ldw	y,c_lreg+2
	ldw	x,c_lreg
	jrpl	pos1		; positive, ok
	cplw	x		; else
	negw	y		; invert
	jrne	pos1
	incw	x
pos1:
	swapw	x
	ldw	c_lreg+1,x	; keep copy
	tnz	c_lreg+2	; if MSB nul
	jrne	posit		; shift right
	ldw	c_lreg+2,y	; otherwise
	jra	left		; shift left
posit:
	swapw	x
pos2:
	inc	a		; update exponent
	srlw	x		; shift value
	rrcw	y
	rrc	c_lreg+2
	jrne	posit		; loop again
	exg	a,xl
	ld	c_lreg+1,a
	exg	a,xl
	ldw	c_lreg+2,y
	ldw	x,c_lreg+1
panor:
	sllw	x		; remove hidden bit
	add	a,#127		; add offset
	sll	c_lreg		; get sign
	rrc	a		; insert sign
	rrcw	x		; align mantissa
	ldw	c_lreg+1,x	; store
	ld	c_lreg,a	; result
fini:
	popw	y		; restore
	popw	x		; registers
	retf			; and return
;
	end
