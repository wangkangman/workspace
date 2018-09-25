;	INTEGER SQUARE ROOT
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_isqrt
	xref.b	c_x
	.dcall	"4,0,f_isqrt"
;
tabhi:
	dc.b	$40,$80,$10,$40,$04,$20,$01,$10
tablo:
	dc.b	$40,$08,$10,$04,$04,$02,$01,$01
;
f_isqrt:
	ldw	c_x,x		; save value
	jreq	fini		; zero, exit
	ld	a,c_x		; check MSB
	jrne	nlow		; go to set mask
	ld	a,c_x+1		; set search
	ld	c_x+2,a		; value
	ldw	x,#8		; scan tablo
	jra	inl1		; jump in loop
nlow:
	ld	c_x+2,a		; set search value
	clrw	x		; scan tabhi
	jra	inl1		; jump in loop
bcl1:
	incw	x		; next entry
	incw	x
inl1:
	ldf	a,(tabhi,x)	; find nearest value
	cp	a,c_x+2
	jrugt	bcl1		; loop while too high
	ldf	a,(tabhi+1,x)	; read starting mask
	ld	c_x+2,a		; store in memory
	jra	inl2		; jump in the loop
bcl2:
	add	a,c_x+2		; set bit
inl2:
	ld	xl,a		; in order to
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
	retf			; and return
;
	end
