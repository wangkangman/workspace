;	LONG MULTIPLY IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	c_flgmul
	xref	c_flmul, c_rtofl
	xref.b	c_x
	.dcall	"9,0,c_flgmul"
;
c_flgmul:
	pushw	x		; save
	push	c_x		; address
	call	c_flmul		; operate
	pop	c_x		; restore
	popw	x		; address
	jp	c_rtofl		; copy result and return
;
	end
