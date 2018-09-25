;	LONG AND
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	c_land
	xref.b	c_lreg
	.dcall	"2,0,c_land"
;
c_land:
	ld	a,c_lreg	; start with MSB
	and	a,(x)		; operate
	ld	c_lreg,a	; store result
	ld	a,c_lreg+1	; continue
	and	a,(1,x)		; operate
	ld	c_lreg+1,a	; store result
	ld	a,c_lreg+2	; continue
	and	a,(2,x)		; operate
	ld	c_lreg+2,a	; store result
	ld	a,c_lreg+3	; end with LSB
	and	a,(3,x)		; operate
	ld	c_lreg+3,a	; store result
	ret			; and return
;
	end
