;	MATH ASSIST FUNCTIONS
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f__addexp
	xdef	f__poly
	xdef	f__unpack
	xref	d_fadd, d_fmul, d_ltor
	xref.b	c_lreg, c_x
;
;	FLOAT _addexp(n, f)	add to binary exponent
;
	.dcall	"4,4,f__addexp"
f__addexp:
	ldw	c_x,x		; copy exponent
	ldw	x,(4,sp)	; test float
	jreq	zero		; nul, out
	sllw	x		; shift to align
	clr	a
	exg	a,xl
	swapw	x		; prepare and
	addw	x,c_x		; add exponent
	swapw	x
	exg	a,xl
	tnz	a		; test result MSB
	jreq	ok		; no over/underflow
	jrpl	sat		; overflow, saturate
zer:
	clrw	x		; underflow
zero:
	ldw	c_lreg,x	; clear result
	ldw	c_lreg+2,x
	retf			; and return
sat:
	ld	a,#255		; saturate
	ld	xh,a
	jra	ovok
ok:
	ld	a,xh		; test if zero
	tnz	a
	jreq	zer
ovok:
	srlw	x		; exponent
	ldw	c_lreg,x	; in place
	ldw	x,(6,sp)	; complete
	ldw	c_lreg+2,x	; result
	retf			; and return
;
;	FLOAT _poly(table, n, f)	compute polynomial
;
	.dcall	"7,5,f__poly"
f__poly:
	ldw	y,x		; copy pointer
	callf	d_ltor		; copy first element
bcl:
	ldw	x,sp
	addw	x,#5
	callf	d_fmul
	addw	y,#4		; increment address
	ldw	x,y
	callf	d_fadd		; addition
	dec	(4,sp)		; count down
	jrne	bcl		; and loop back
	retf			; return
;
;	COUNT _unpack(pd)	unpack binary exponent
;
	.dcall	"5,0,f__unpack"
f__unpack:
	ldw	c_x,x		; copy pointer
	ldw	x,(x)		; test zero
	jreq	ufin		; nul, exit
	sllw	x		; get exponent
	ld	a,#$7E		; set new
	swapw	x		; exchange
	exg	a,xl		; with
	swapw	x		; exponent
	rrcw	x		; align with sign
	ldw	y,c_x		; get pointer
	ldw	(y),x		; store result
	clrw	x		; prepare
	ld	xl,a		; result
	subw	x,#126		; remove offset
ufin:
	retf			; and return
;
	end
