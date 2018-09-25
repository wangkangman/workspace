;	MOVE SIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- integer in Y
;	- result in LONG ACCU
;
	xdef	c_itoly
	xref.b	c_lreg
	.dcall	"2,0,c_itoly"
;
c_itoly:
	ldw	c_lreg+2,y	; store LSW
	jrpl	posit		; positive, clear
	ldw	y,#-1		; otherwise
	ldw	c_lreg,y	; propagate sign
	ret			; and return
posit:
	clrw	y		; otherwise
	ldw	c_lreg,y	; clear result
	ret			; and return
;
	end
