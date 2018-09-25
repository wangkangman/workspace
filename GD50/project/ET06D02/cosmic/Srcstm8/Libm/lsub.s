;	LONG SUBSTRACT
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	d_lsub
	xref.b	c_lreg
	.dcall	"3,0,d_lsub"
;
d_lsub:
	ld	a,c_lreg+3	; start with LSB
	sub	a,(3,x)		; substract and
	ld	c_lreg+3,a	; store result
	ld	a,c_lreg+2	; continue
	sbc	a,(2,x)		; propagate carry
	ld	c_lreg+2,a	; store result
	ld	a,c_lreg+1	; continue
	sbc	a,(1,x)		; propagate carry
	ld	c_lreg+1,a	; store result
	ld	a,c_lreg	; end with MSB
	sbc	a,(x)		; propagate carry
	ld	c_lreg,a	; store result
	retf			; and return
;
	end
