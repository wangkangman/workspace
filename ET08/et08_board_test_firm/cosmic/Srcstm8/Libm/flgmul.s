;	LONG MULTIPLY IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	d_flgmul
	xref	d_flmul, d_rtofl
	xref.b	c_x
	.dcall	"11,0,d_flgmul"
;
d_flgmul:
	pushw	x		; save
	push	c_x		; address
	callf	d_flmul		; operate
	pop	c_x		; restore
	popw	x		; address
	jpf	d_rtofl		; copy result and return
;
	end
