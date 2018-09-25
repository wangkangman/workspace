;	LONG AND FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in ACCU
;	- 2nd arg pointed by X and extension
;
	xdef	c_fland
	xref.b	c_lreg, c_x
	.dcall	"2,0,c_fland"
;
c_fland:
	ldw	c_x+1,x		; store address LSB
	clrw	x		; start with MSB
	ldf	a,([c_x.e],x)	; load
	and	a,c_lreg	; operate
	ld	c_lreg,a	; store result
	incw	x		; next byte
	ldf	a,([c_x.e],x)	; load
	and	a,c_lreg+1	; operate
	ld	c_lreg+1,a	; store result
	incw	x		; with next byte
	ldf	a,([c_x.e],x)	; load
	and	a,c_lreg+2	; operate
	ld	c_lreg+2,a	; store result
	incw	x		; end with LSB
	ldf	a,([c_x.e],x)	; load
	and	a,c_lreg+3	; operate
	ld	c_lreg+3,a	; store result
	ret			; and return
;
	end
