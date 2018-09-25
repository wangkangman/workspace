;	LONG SUBSTRACT IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	c_ffgsub
	xref	c_ffsub, c_rtofl, c_fneg
	xref.b	c_x
	.dcall	"9,0,c_ffgsub"
;
c_ffgsub:
	pushw	x		; save
	push	c_x		; address
	call	c_ffsub		; operate
	call	c_fneg		; invert result
	pop	c_x		; restore
	popw	x		; address
	jp	c_rtofl		; copy result and return
;
	end
