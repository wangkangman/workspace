;	LONG SUBSTRACT IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	d_flgsub
	xref	d_flsub, d_rtofl2, d_lneg
	.dcall	"6,0,d_flgsub"
;
d_flgsub:
	callf	d_flsub		; operate
	callf	d_lneg		; invert result
	jpf	d_rtofl2	; copy result and return
;
	end
