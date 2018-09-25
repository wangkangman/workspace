;	LONG INTEGER SQUARE ROOT
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_lsqrt
	xref	f_isqrt
	xref.b	c_lreg, c_x, c_y
	.dcall	"3,4,f_lsqrt"
;
tabhi:	equ	f_isqrt-16
;
f_lsqrt:
	ld	a,(4,sp)	; test MSW hi
	jrne	nlow		; not zero, set mask
	ld	a,(5,sp)	; test MSW lo
	jrne	nint		; not zero, set mask
	ldw	x,(6,sp)	; set input value
	jpf	f_isqrt		; and compute result
nint:
	ld	c_x,a		; set search value
	ldw	x,#8		; scan tablo
	jra	inl1		; jump in loop
nlow:
	ld	c_x,a		; set search value
	clrw	x		; scan tabhi
	jra	inl1		; jump in loop
bcl1:
	incw	x		; next entry
	incw	x
inl1:
	ldf	a,(tabhi,x)	; find nearest value
	cp	a,c_x
	jrugt	bcl1		; loop while too low
	ldf	a,(tabhi+1,x)	; read starting mask
	ld	c_x,a		; save value
	clr	c_x+1
	ld	c_y,a		; save mask
	clr	c_y+1
	jra	inl2		; jump in the loop
bcl2:
	ld	a,c_y+1		; set bit
	jreq	nb1
	or	a,c_x+1		; in value
	ld	c_x+1,a
	ld	a,c_x		; prepare value
	jra	inl2
nb1:
	ld	a,c_y
	or	a,c_x
	ld	c_x,a
inl2:
	ld	xl,a
	mul	x,a		; H * H
	ldw	c_lreg,x	; store in accumulator
	ld	a,c_x+1
	ld	xl,a
	mul	x,a		; L * L
	ldw	c_lreg+2,x	; store in accumulator
	ld	a,c_x
	jreq	noc2		; shortcut
	ld	xl,a
	ld	a,c_x+1
	jreq	noc2		; shortcut
	mul	x,a		; H * L
	sllw	x		; shift for twice
	jrnc	noc1		; if carry set
	inc	c_lreg		; propagate
noc1:
	addw	x,c_lreg+1	; accumulate
	ldw	c_lreg+1,x
	jrnc	noc2		; if carry set
	inc	c_lreg		; propagate
noc2:
	ldw	x,c_lreg
	cpw	x,(4,sp)	; compare MSW
	jrult	next		; too low, keep bit
	jrne	rem		; too hig, remove bit
	ldw	x,c_lreg+2
	cpw	x,(6,sp)	; compare LSW
	jreq	ok		; found, exit
	jrult	next		; too low, keep bit
rem:
	ld	a,c_y+1		; remove bit
	jreq	nb2
	xor	a,c_x+1		; in value
	ld	c_x+1,a
	jra	next
nb2:
	ld	a,c_y
	xor	a,c_x
	ld	c_x,a
next:
	srl	c_y		; shift mask
	rrc	c_y+1
	jrnc	bcl2		; loop while bit in
ok:
	ldw	x,c_x		; get result
	retf			; and return
;
	end
