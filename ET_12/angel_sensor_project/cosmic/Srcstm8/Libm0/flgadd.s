;	LONG ADDITION IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	c_flgadd
	xref	c_fladd, c_rtofl2
	.dcall	"4,0,c_flgadd"
;
c_flgadd:
	call	c_fladd		; operate
	jp	c_rtofl2	; copy result and return
;
	end
