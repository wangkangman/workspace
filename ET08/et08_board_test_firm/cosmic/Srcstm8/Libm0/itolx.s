;	MOVE SIGNED INTEGER TO LONG ACCUMULATOR
;	Copyright (c) 2006 by COSMIC Software
;	- integer in X
;	- result in LONG ACCU
;
	xdef	c_itolx
	xref.b	c_lreg
	.dcall	"2,0,c_itolx"
;
c_itolx:
	ldw	c_lreg+2,x	; store LSW
	jrpl	posit		; positive, clear
	ldw	x,#-1		; otherwise
	ldw	c_lreg,x	; propagate sign
	ret			; and return
posit:
	clrw	x		; otherwise
	ldw	c_lreg,x	; clear result
	ret			; and return
;
	end
