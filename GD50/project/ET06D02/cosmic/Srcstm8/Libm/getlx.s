;	GET A LONG WORD FROM EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in X
;	- result in LONG ACCU
;
	xdef	d_getlx
	xref.b	c_x, c_lreg
	.dcall	"3,0,d_getlx"
;
d_getlx:
	ld	a,(x)		; get MSB
	ld	c_lreg,a	; store MSB
	ld	a,(1,x)		; get it
	ld	c_lreg+1,a	; store
	ld	a,(2,x)		; get it
	ld	c_lreg+2,a	; store
	ld	a,(3,x)		; get it
	ld	c_lreg+3,a	; store LSB
	retf			; and return
;
	end
