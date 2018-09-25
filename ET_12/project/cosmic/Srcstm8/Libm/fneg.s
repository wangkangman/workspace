;	FLOAT NEGATE
;	Copyright (c) 2006 by COSMIC Software
;	- operand and result in FLOAT ACCU
;
	xdef	d_fneg
	xref.b	c_lreg
	.dcall	"3,0,d_fneg"
;
d_fneg:
	ld	a,c_lreg	; test MSB
	or	a,c_lreg+1	; test next byte
	jreq	fini		; nul, exit
ok:
	bcpl	c_lreg,#7	; invert sign
fini:
	retf			; and return
;
	end
