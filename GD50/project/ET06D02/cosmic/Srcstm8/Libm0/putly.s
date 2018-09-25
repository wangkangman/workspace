;	PUT A LONG WORD IN EXTERNAL MEMORY
;	Copyright (c) 2001 by COSMIC Software
;	- address in Y and extension
;	- operand in LONG ACCU
;
	xdef	c_putly
	xref.b	c_y, c_lreg
	.dcall	"2,0,c_putly"
;
c_putly:
	exgw	x,y		; for efficiency
	ld	a,c_lreg	; get MSB
	ld	(x),a		; store MSB
	ld	a,c_lreg+1	; get byte
	ld	(1,x),a		; store it
	ld	a,c_lreg+2	; get byte
	ld	(2,x),a		; store it
	ld	a,c_lreg+3	; get LSB
	ld	(3,x),a		; store it
	exgw	x,y		; restore registers
	ret			; and return
;
	end
