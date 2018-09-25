;	LONG SUBSTRACT IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	d_ffgsub
	xref	d_ffsub, d_rtofl, d_fneg
	xref.b	c_x
	.dcall	"11,0,d_ffgsub"
;
d_ffgsub:
	pushw	x		; save
	push	c_x		; address
	callf	d_ffsub		; operate
	callf	d_fneg		; invert result
	pop	c_x		; restore
	popw	x		; address
	jpf	d_rtofl		; copy result and return
;
	end
