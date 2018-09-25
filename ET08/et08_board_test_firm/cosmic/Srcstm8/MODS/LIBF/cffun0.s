;	MATH ASSIST FUNCTIONS
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	__addexp
	xdef	__poly
	xdef	__unpack
	xref	c_fadd, c_fmul, c_ltor
	xref.b	c_lreg, c_x
;
;	FLOAT _addexp(n, f)	add to binary exponent
;
	.dcall	"2,4,__addexp"
__addexp:
	ldw	c_x,x		; copy exponent
	ldw	x,(3,sp)	; test float
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
	ret			; and return
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
	ldw	x,(5,sp)	; complete
	ldw	c_lreg+2,x	; result
	ret			; and return
;
;	FLOAT _poly(table, n, f)	compute polynomial
;
	.dcall	"6,5,__poly"
__poly:
	ldw	y,x		; copy pointer
	call	c_ltor		; copy first element
bcl:
	ldw	x,sp
	addw	x,#4
	call	c_fmul
	addw	y,#4		; increment address
	ldw	x,y
	call	c_fadd		; addition
	dec	(3,sp)		; count down
	jrne	bcl		; and loop back
	ret			; return
;
;	COUNT _unpack(pd)	unpack binary exponent
;
	.dcall	"4,0,__unpack"
__unpack:
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
	ret			; and return
;
	end
