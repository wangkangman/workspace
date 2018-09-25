;	LONG ADDITION IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	d_flgadd
	xref	d_fladd, d_rtofl2
	.dcall	"6,0,d_flgadd"
;
d_flgadd:
	callf	d_fladd		; operate
	jpf	d_rtofl2	; copy result and return
;
	end
