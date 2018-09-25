;	LONG COMPARE AGAINST ZERO
;	Copyright (c) 2006 by COSMIC Software
;	- operand pointed by X
;
	xdef	d_lzmp
	.dcall	"3,0,d_lzmp"
;
d_lzmp:
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
	retf			; and return
;
	end
