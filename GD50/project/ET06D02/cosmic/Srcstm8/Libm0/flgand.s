;	LONG AND IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	c_flgand
	xref	c_fland, c_rtofl2
	.dcall	"4,0,c_flgand"
;
c_flgand:
	call	c_fland		; operate
	jp	c_rtofl2	; copy result and return
;
	end
