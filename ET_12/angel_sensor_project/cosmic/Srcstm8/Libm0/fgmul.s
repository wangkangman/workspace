;	FLOAT MULTIPLICATION IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	c_fgmul
	xref	c_fmul, c_rtol
	.dcall	"6,0,c_fgmul"
;
c_fgmul:
	pushw	x		; save address
	call	c_fmul		; operate
	popw	x		; take back address
	jp	c_rtol		; copy result and return
;
	end
