;	LONG AND IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	d_flgand
	xref	d_fland, d_rtofl2
	.dcall	"6,0,d_flgand"
;
d_flgand:
	callf	d_fland		; operate
	jpf	d_rtofl2	; copy result and return
;
	end
