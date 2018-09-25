;	FLOAT MULTIPLICATION IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	c_ffgmul
	xref	c_ffmul, c_rtofl
	xref.b	c_x
	.dcall	"9,0,c_ffgmul"
;
c_ffgmul:
	pushw	x		; save operand address
	push	c_x
	call	c_ffmul		; operate
	pop	c_x		; restore operand address
	popw	x
	jp	c_rtofl		; copy result and return
;
	end
