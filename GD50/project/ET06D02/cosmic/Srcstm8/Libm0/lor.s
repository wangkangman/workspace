;	LONG OR
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	c_lor
	xref.b	c_lreg
	.dcall	"2,0,c_lor"
;
c_lor:
	ld	a,c_lreg	; start with MSB
	or	a,(x)		; operate
	ld	c_lreg,a	; store result
	ld	a,c_lreg+1	; continue
	or	a,(1,x)		; operate
	ld	c_lreg+1,a	; store result
	ld	a,c_lreg+2	; continue
	or	a,(2,x)		; operate
	ld	c_lreg+2,a	; store result
	ld	a,c_lreg+3	; end with LSB
	or	a,(3,x)		; operate
	ld	c_lreg+3,a	; store result
	ret			; and return
;
	end
