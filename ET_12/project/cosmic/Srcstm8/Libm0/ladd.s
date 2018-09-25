;	LONG ADDITION
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	c_ladd
	xref.b	c_lreg
	.dcall	"2,0,c_ladd"
;
c_ladd:
	ld	a,c_lreg+3	; start with LSB
	add	a,(3,x)		; add and
	ld	c_lreg+3,a	; store result
	ld	a,c_lreg+2	; continue
	adc	a,(2,x)		; propagate carry
	ld	c_lreg+2,a	; store result
	ld	a,c_lreg+1	; continue
	adc	a,(1,x)		; propagate carry
	ld	c_lreg+1,a	; store result
	ld	a,c_lreg	; end with MSB
	adc	a,(x)		; propagate carry
	ld	c_lreg,a	; store result
	ret			; and return
;
	end
