;	FAR POINTER ADDITION IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in LONG ACCU
;
	xdef	c_pgadd
	xref.b	c_lreg
	.dcall	"2,0,c_pgadd"
;
c_pgadd:
	ld	a,(2,x)		; start with LSB
	add	a,c_lreg+3	; add and
	ld	(2,x),a		; store result
	ld	a,(1,x)		; continue
	adc	a,c_lreg+2	; propagate carry
	ld	(1,x),a		; store result
	ld	a,(x)		; continue
	adc	a,c_lreg+1	; propagate carry
	ld	(x),a		; store result
	ret			; and return
;
	end
