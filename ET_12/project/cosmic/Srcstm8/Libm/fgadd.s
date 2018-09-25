;	FLOAT ADDITION IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	d_fgadd
	xref	d_fadd, d_rtol
	.dcall	"10,0,d_fgadd"
;
d_fgadd:
	pushw	x		; save address
	callf	d_fadd		; operate
	popw	x		; take back address
	jpf	d_rtol		; copy result and return
;
	end
