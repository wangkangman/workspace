;	LONG COMPARE AGAINST ZERO FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X and extension
;
	xdef	d_flzmp
	xref.b	c_x
	.dcall	"3,0,d_flzmp"
;
d_flzmp:
	clr	c_x+2		; set address to keep X
	clr	c_x+1
	ldf	a,([c_x.e],x)	; test MSB
	jrne	fini		; not equal, flags set
	inc	c_x+2		; next byte
	ldf	a,([c_x.e],x)
	jrne	ok		; not equal, set flags
	inc	c_x+2		; next byte
	ldf	a,([c_x.e],x)
	jrne	ok		; not equal, set flags
	inc	c_x+2		; next byte
	ldf	a,([c_x.e],x)
	jreq	fini		; equal, flags set
ok:
	ld	a,#1		; clear N and Z, keep C
fini:
	retf			; and return
;
	end
