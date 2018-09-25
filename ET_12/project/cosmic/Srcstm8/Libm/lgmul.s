;	LONG MULTIPLY IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in ACCU
;
	xdef	d_lgmul
	xref	d_lmul, d_rtol
	.dcall	"7,0,d_lgmul"
;
d_lgmul:
	pushw	x		; save address
	callf	d_lmul		; operate
	popw	x		; get back address
	jpf	d_rtol		; copy and return
;
	end
