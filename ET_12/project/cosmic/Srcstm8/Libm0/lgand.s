;	LONG AND IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	c_lgand
	xref.b	c_lreg
	.dcall	"2,0,c_lgand"
;
c_lgand:
	ld	a,c_lreg	; start with MSB
	and	a,(x)		; operate
	ld	(x),a		; store result
	ld	a,c_lreg+1	; continue
	and	a,(1,x)		; operate
	ld	(1,x),a		; store result
	ld	a,c_lreg+2	; continue
	and	a,(2,x)		; operate
	ld	(2,x),a		; store result
	ld	a,c_lreg+3	; end with LSB
	and	a,(3,x)		; operate
	ld	(3,x),a		; store result
	ret			; and return
;
	end
