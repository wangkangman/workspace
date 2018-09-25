;	GET A LONG WORD FROM EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y and extension
;	- result in LONG ACCU
;
	xdef	d_getly
	xref.b	c_y, c_lreg
	.dcall	"3,0,d_getly"
;
d_getly:
	ld	a,(y)		; get MSB
	ld	c_lreg,a	; store MSB
	ld	a,(1,y)		; get it
	ld	c_lreg+1,a	; store
	ld	a,(2,y)		; get it
	ld	c_lreg+2,a	; store
	ld	a,(3,y)		; get it
	ld	c_lreg+3,a	; store LSB
	retf			; and return
;
	end
