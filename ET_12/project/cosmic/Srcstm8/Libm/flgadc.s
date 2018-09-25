;	LONG ADDITION IN FAR MEMORY WITH BYTE
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in A
;
	xdef	d_flgadc
	xref.b	c_x
	.dcall	"4,0,d_flgadc"
;
d_flgadc:
	ldw	c_x+1,x		; store address LSB
	ldw	x,#3		; LSB index
	push	a		; save value
	ldf	a,([c_x.e],x)	; get operand and
	add	a,(1,sp)	; add value
next:
	ldf	([c_x.e],x),a	; store result
	jrnc	ok		; no carry, exit
	decw	x		; next byte
	jrmi	ok		; last, exit
	ldf	a,([c_x.e],x)	; propagate
	add	a,#1		; carry
	jra	next		; next byte
ok:
	pop	a		; clean stack
	ldw	x,c_x+1		; restore register
	retf			; and return
;
	end
