;	PUT A LONG WORD IN EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in X
;	- operand in LONG ACCU
;
	xdef	d_putlx
	xref.b	c_x, c_lreg
	.dcall	"3,0,d_putlx"
;
d_putlx:
	ld	a,c_lreg	; get MSB
	ld	(x),a		; store MSB
	ld	a,c_lreg+1	; get byte
	ld	(1,x),a		; store it
	ld	a,c_lreg+2	; get byte
	ld	(2,x),a		; store it
	ld	a,c_lreg+3	; get LSB
	ld	(3,x),a		; store it
	retf			; and return
;
	end
