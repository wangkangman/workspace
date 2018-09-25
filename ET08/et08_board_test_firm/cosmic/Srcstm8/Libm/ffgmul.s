;	FLOAT MULTIPLICATION IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	d_ffgmul
	xref	d_ffmul, d_rtofl
	xref.b	c_x
	.dcall	"11,0,d_ffgmul"
;
d_ffgmul:
	pushw	x		; save operand address
	push	c_x
	callf	d_ffmul		; operate
	pop	c_x		; restore operand address
	popw	x
	jpf	d_rtofl		; copy result and return
;
	end
