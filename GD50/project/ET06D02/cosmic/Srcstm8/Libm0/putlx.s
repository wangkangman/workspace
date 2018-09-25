;	PUT A LONG WORD IN EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in X
;	- operand in LONG ACCU
;
	xdef	c_putlx
	xref.b	c_x, c_lreg
	.dcall	"2,0,c_putlx"
;
c_putlx:
	ld	a,c_lreg	; get MSB
	ld	(x),a		; store MSB
	ld	a,c_lreg+1	; get byte
	ld	(1,x),a		; store it
	ld	a,c_lreg+2	; get byte
	ld	(2,x),a		; store it
	ld	a,c_lreg+3	; get LSB
	ld	(3,x),a		; store it
	ret			; and return
;
	end
