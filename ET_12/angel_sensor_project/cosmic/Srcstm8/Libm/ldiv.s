;	SIGNED/UNSIGNED LONG DIVISION
;	Copyright (c) 2007 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg and accu pointed by X
;
	xdef	d_divsl, d_divul
	xref	d_lneg, d_lgneg
	xref.b	c_lreg, c_x
;
d_divsl:
	clr	c_x		; clear sign flag
	tnz	a		; test operand
	jrpl	pos		; operand positive
	callf	d_lgneg		; invert
	bset	c_x,#0		; and mark sign
pos:
	ld	a,c_lreg	; test dividend
	jrpl	d_divul		; positive, ok
	callf	d_lneg		; else invert
	bset	c_x,#1		; and mark sign
d_divul:
	pushw	y		; save register
	ld	a,(x)		; test divisor HH
	jrne	short		; high enough, ok
	ld	a,(1,x)		; test divisor HL
	jrne	short		; high enough, ok
	ldw	y,c_lreg	; high word dividend
	jreq	single		; zero, single divide
	ld	a,(2,x)		; test divisor LH
	jreq	fast		; zero, 8 bit division
	cp	a,c_lreg	; compare with dividend HH
	jrugt	short2		; high enough, ok
	jrne	long		; else
	ld	a,(3,x)		; test divisor LL
	cp	a,c_lreg+1	; with dividend HL
	jrugt	short2		; high enough, ok
long:
	ldw	y,#32		; shift count
	clr	a
	ld	(6,x),a
	ld	(7,x),a
	jra	cont		; and continue
single:
	pushw	x		; save address
	ldw	x,(2,x)		; divisor
	ldw	y,c_lreg+2	; dividend
	exgw	x,y		; in place
	divw	x,y		; divide
	ldw	c_lreg+2,x	; quotient in place
	popw	x		; restore pointer
	ldw	(6,x),y		; remainder in place
	clrw	y		; complete
	ldw	(4,x),y		; remainder
	ldw	c_lreg,y	; and quotient
	popw	y
	retf			; and return
fast:
	ld	(4,x),a		; prepare remainder
	ld	(5,x),a
	ld	(6,x),a
	ld	a,(3,x)		; load divisor
	div	y,a		; HH-HL
	ldw	c_lreg,y
	ld	yh,a
	ld	a,c_lreg+2	; LH
	ld	yl,a
	ld	a,(3,x)		; load divisor
	div	y,a
	rrwa	y,a
	ld	c_lreg+2,a
	ld	a,c_lreg+3	; LL
	ld	yl,a
	ld	a,(3,x)		; load divisor
	div	y,a
	ld	(7,x),a		; store remainder
	ld	a,yl
	ld	c_lreg+3,a
	popw	y
	retf			; and return
short:
	ldw	y,c_lreg	; shift by 16
short2:
	ldw	(6,x),y
	ldw	y,c_lreg+2
	ldw	c_lreg,y
	ldw	y,#16		; shift count
	clr	a
	ld	c_lreg+2,a
	ld	c_lreg+3,a
cont:
	ld	(5,x),a		; clear quotient
bcl:
	sll	c_lreg+3	; shift rem + quo
	rlc	c_lreg+2
	rlc	c_lreg+1
	rlc	c_lreg
	rlc	(7,x)
	rlc	(6,x)
	rlc	(5,x)
	rlc	a
	cp	a,(x)		; compare with divisor
	jrult	next		; less, continue
	jrne	gosubs		; greater, substact
	ld	(4,x),a		; save MSB
	ld	a,(5,x)
	cp	a,(1,x)
	jrult	nextl		; less, continue
	jrne	gosub		; greater, substact
	ld	a,(6,x)
	cp	a,(2,x)
	jrult	nextl		; less, continue
	jrne	gosub		; greater, substact
	ld	a,(7,x)
	sub	a,(3,x)
	jruge	suite		; ok, continue operation
nextl:
	ld	a,(4,x)		; load MSB
	jra	next		; and continue
gosubs:
	ld	(4,x),a		; store MSB
gosub:
	ld	a,(7,x)
	sub	a,(3,x)		; long substract
suite:
	ld	(7,x),a
	ld	a,(6,x)
	sbc	a,(2,x)
	ld	(6,x),a
	ld	a,(5,x)
	sbc	a,(1,x)
	ld	(5,x),a
	ld	a,(4,x)
	sbc	a,(x)		; keep MSB in A
	inc	c_lreg+3	; set result bit
next:
	decw	y		; decrement counter
	jrne	bcl		; and loop back
	popw	y		; restore register
	ld	(4,x),a		; complete remainder
	retf			; return
;
	end
