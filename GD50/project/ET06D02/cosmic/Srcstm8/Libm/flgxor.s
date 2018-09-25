;	LONG EXCLUSIVE OR IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	d_flgxor
	xref	d_flxor, d_rtofl2
	.dcall	"6,0,d_flgxor"
;
d_flgxor:
	callf	d_flxor		; operate
	jpf	d_rtofl2	; copy result and return
;
	end
