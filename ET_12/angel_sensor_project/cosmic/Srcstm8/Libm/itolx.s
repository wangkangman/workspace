;	MOVE SIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- integer in X
;	- result in LONG ACCU
;
	xdef	d_itolx
	xref.b	c_lreg
	.dcall	"3,0,d_itolx"
;
d_itolx:
	ldw	c_lreg+2,x	; store LSW
	jrpl	posit		; positive, clear
	ldw	x,#-1		; otherwise
	ldw	c_lreg,x	; propagate sign
	retf			; and return
posit:
	clrw	x		; otherwise
	ldw	c_lreg,x	; clear result
	retf			; and return
;
	end
