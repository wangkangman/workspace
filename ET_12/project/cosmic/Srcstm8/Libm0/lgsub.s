;	LONG SUBSTRACT IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in LONG ACCU
;
	xdef	c_lgsub
	xref.b	c_lreg
	.dcall	"2,0,c_lgsub"
;
c_lgsub:
	ld	a,(3,x)		; start with LSB
	sub	a,c_lreg+3	; substract and
	ld	(3,x),a		; store result
	ld	a,(2,x)		; continue
	sbc	a,c_lreg+2	; propagate carry
	ld	(2,x),a		; store result
	ld	a,(1,x)		; continue
	sbc	a,c_lreg+1	; propagate carry
	ld	(1,x),a		; store result
	ld	a,(x)		; end with MSB
	sbc	a,c_lreg	; propagate carry
	ld	(x),a		; store result
	ret			; and return
;
	end
