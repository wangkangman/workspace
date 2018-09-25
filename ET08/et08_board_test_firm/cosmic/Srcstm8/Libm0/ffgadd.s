;	FLOAT ADDITION IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X
;	- 2nd arg in FLOAT ACCU
;
	xdef	c_ffgadd
	xref	c_ffadd, c_rtofl
	xref.b	c_x
	.dcall	"9,0,c_ffgadd"
;
c_ffgadd:
	pushw	x		; save
	push	c_x		; address
	call	c_ffadd		; operate
	pop	c_x		; restore
	popw	x		; address
	jp	c_rtofl		; copy result and return
;
	end
