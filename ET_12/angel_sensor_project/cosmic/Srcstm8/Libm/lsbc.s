;	LONG SUBSTRACT WITH BYTE
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg in A
;
	xdef	d_lsbc
	xref.b	c_lreg
	.dcall	"3,0,d_lsbc"
;
d_lsbc:
	neg	a		; invert input
	add	a,c_lreg+3	; start with LSB
	ld	c_lreg+3,a	; store result
	jrc	ok		; carry, exit
	ld	a,c_lreg+2	; continue
	dec	c_lreg+2	; next byte
	tnz	a		; check byte
	jrne	ok		; not zero, exit
	ld	a,c_lreg+1	; next byte
	jrne	okl		; not zero, skip last
	dec	c_lreg		; last byte
okl:
	dec	c_lreg+1	; store result
ok:
	retf			; and return
;
	end
