;	FLOAT SUBSTRACT IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	c_fgsub
	xref	c_fsub, c_fneg, c_rtol
	.dcall	"8,0,c_fgsub"
;
c_fgsub:
	pushw	x		; save address
	call	c_fsub		; operate
	call	c_fneg		; negate result
	popw	x		; take back address
	jp	c_rtol		; copy result and return
;
	end
