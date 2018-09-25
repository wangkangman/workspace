;	LONG ADDITION IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in LONG ACCU
;
	xdef	d_lgadd
	xref.b	c_lreg
	.dcall	"3,0,d_lgadd"
;
d_lgadd:
	ld	a,(3,x)		; start with LSB
	add	a,c_lreg+3	; add and
	ld	(3,x),a		; store result
	ld	a,(2,x)		; continue
	adc	a,c_lreg+2	; propagate carry
	ld	(2,x),a		; store result
	ld	a,(1,x)		; continue
	adc	a,c_lreg+1	; propagate carry
	ld	(1,x),a		; store result
	ld	a,(x)		; end with MSB
	adc	a,c_lreg	; propagate carry
	ld	(x),a		; store result
	retf			; and return
;
	end
