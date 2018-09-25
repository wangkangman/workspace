;	LONG EXCLUSIVE OR
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	d_lxor
	xref.b	c_lreg
	.dcall	"3,0,d_lxor"
;
d_lxor:
	ld	a,c_lreg	; start with MSB
	xor	a,(x)		; operate
	ld	c_lreg,a	; store result
	ld	a,c_lreg+1	; continue
	xor	a,(1,x)		; operate
	ld	c_lreg+1,a	; store result
	ld	a,c_lreg+2	; continue
	xor	a,(2,x)		; operate
	ld	c_lreg+2,a	; store result
	ld	a,c_lreg+3	; end with LSB
	xor	a,(3,x)		; operate
	ld	c_lreg+3,a	; store result
	retf			; and return
;
	end
