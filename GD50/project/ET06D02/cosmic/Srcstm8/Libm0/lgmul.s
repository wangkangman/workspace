;	LONG MULTIPLY IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in ACCU
;
	xdef	c_lgmul
	xref	c_lmul, c_rtol
	.dcall	"5,0,c_lgmul"
;
c_lgmul:
	pushw	x		; save address
	call	c_lmul		; operate
	popw	x		; get back address
	jp	c_rtol		; copy and return
;
	end
