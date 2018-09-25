;	FLOAT ADDITION IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	d_ffgadd
	xref	d_ffadd, d_rtofl
	xref.b	c_x
	.dcall	"11,0,d_ffgadd"
;
d_ffgadd:
	pushw	x		; save
	push	c_x		; address
	callf	d_ffadd		; operate
	pop	c_x		; restore
	popw	x		; address
	jpf	d_rtofl		; copy result and return
;
	end
