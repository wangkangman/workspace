;	INTEGER SQUARE ROOT
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_isqrt
	xref.b	c_x
	.dcall	"3,0,_isqrt"
;
tabhi:
	dc.b	$40,$80,$10,$40,$04,$20,$01,$10
tablo:
	dc.b	$40,$08,$10,$04,$04,$02,$01,$01
;
_isqrt:
	ldw	c_x,x		; if value zero
	jreq	fini		; exit
	ld	a,c_x		; check MSB
	jrne	nlow		; go to set mask
	ld	a,c_x+1		; set search value
	ldw	x,#tablo	; scan tablo
	jra	inl1		; jump in loop
nlow:
	ldw	x,#tabhi	; scan tabhi
	jra	inl1		; jump in loop
bcl1:
	incw	x		; next entry
	incw	x
inl1:
	cp	a,(x)		; find nearest value
	jrult	bcl1		; loop while too high
	ld	a,(1,x)		; read starting mask
	ld	c_x+2,a		; store in memory
	jra	inl2		; jump in the loop
bcl2:
	add	a,c_x+2		; set bit
inl2:
	ld	xl,a		; multiply to
	mul	x,a		; get approximation
	cpw	x,c_x		; compare with value
	jreq	ok		; found, exit
	jrult	next		; too low, keep bit
	sub	a,c_x+2		; remove bit
next:
	srl	c_x+2		; shift mask
	jrne	bcl2		; loop while bit set
ok:
	clrw	x		; for int
	ld	xl,a		; result
fini:
	ret			; and return
;
	end
