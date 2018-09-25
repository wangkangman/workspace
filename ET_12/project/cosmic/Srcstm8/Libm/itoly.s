;	MOVE SIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- integer in Y
;	- result in LONG ACCU
;
	xdef	d_itoly
	xref.b	c_lreg
	.dcall	"3,0,d_itoly"
;
d_itoly:
	ldw	c_lreg+2,y	; store LSW
	jrpl	posit		; positive, clear
	ldw	y,#-1		; otherwise
	ldw	c_lreg,y	; propagate sign
	retf			; and return
posit:
	clrw	y		; otherwise
	ldw	c_lreg,y	; clear result
	retf			; and return
;
	end
