;	FLOAT MULTIPLICATION IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	d_fgmul
	xref	d_fmul, d_rtol
	.dcall	"8,0,d_fgmul"
;
d_fgmul:
	pushw	x		; save address
	callf	d_fmul		; operate
	popw	x		; take back address
	jpf	d_rtol		; copy result and return
;
	end
