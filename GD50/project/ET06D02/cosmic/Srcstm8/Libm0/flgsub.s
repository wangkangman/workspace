;	LONG SUBSTRACT IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	c_flgsub
	xref	c_flsub, c_rtofl2, c_lneg
	.dcall	"4,0,c_flgsub"
;
c_flgsub:
	call	c_flsub		; operate
	call	c_lneg		; invert result
	jp	c_rtofl2	; copy result and return
;
	end
