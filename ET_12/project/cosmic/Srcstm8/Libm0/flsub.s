;	LONG SUBSTRACT FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X and extension
;
	xdef	c_flsub
	xref	c_lneg
	xref.b	c_lreg, c_x
	.dcall	"2,0,c_flsub"
;
c_flsub:
	ldw	c_x+1,x		; store address LSW
	ldw	x,#3		; LSB index
	ldf	a,([c_x.e],x)	; start with LSB
	sub	a,c_lreg+3	; operate
	ld	c_lreg+3,a	; store result
	decw	x		; next byte
	ldf	a,([c_x.e],x)	; load
	sbc	a,c_lreg+2	; operate
	ld	c_lreg+2,a	; store result
	decw	x		; next byte
	ldf	a,([c_x.e],x)	; load
	sbc	a,c_lreg+1	; operate
	ld	c_lreg+1,a	; store result
	ldf	a,[c_x.e]	; load
	sbc	a,c_lreg	; operate
	ld	c_lreg,a	; store result
	jp	c_lneg		; invert and return
;
	end
