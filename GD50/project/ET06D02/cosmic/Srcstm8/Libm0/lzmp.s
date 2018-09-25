;	LONG COMPARE AGAINST ZERO
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X
;
	xdef	c_lzmp
	.dcall	"2,0,c_lzmp"
;
c_lzmp:
	tnz	(x)		; test MSB
	jrne	fini		; not equal, flags set
	tnz	(1,x)
	jrne	ok		; not equal, set flags
	tnz	(2,x)
	jrne	ok		; not equal, set flags
	tnz	(3,x)
	jreq	fini		; equal, flags set
ok:
	ld	a,#1		; clear N and Z, keep C
fini:
	ret			; and return
;
	end
