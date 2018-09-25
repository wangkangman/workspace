;	FLOAT ADDITION IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	c_fgadd
	xref	c_fadd, c_rtol
	.dcall	"8,0,c_fgadd"
;
c_fgadd:
	pushw	x		; save address
	call	c_fadd		; operate
	popw	x		; take back address
	jp	c_rtol		; copy result and return
;
	end
