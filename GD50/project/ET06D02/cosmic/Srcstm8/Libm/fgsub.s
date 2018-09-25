;	FLOAT SUBSTRACT IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	d_fgsub
	xref	d_fsub, d_fneg, d_rtol
	.dcall	"10,0,d_fgsub"
;
d_fgsub:
	pushw	x		; save address
	callf	d_fsub		; operate
	callf	d_fneg		; negate result
	popw	x		; take back address
	jpf	d_rtol		; copy result and return
;
	end
