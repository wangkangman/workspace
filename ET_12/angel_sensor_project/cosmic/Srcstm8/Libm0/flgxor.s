;	LONG EXCLUSIVE OR IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result pointed by X and extension
;	- 2nd arg in LONG ACCU
;
	xdef	c_flgxor
	xref	c_flxor, c_rtofl2
	.dcall	"4,0,c_flgxor"
;
c_flgxor:
	call	c_flxor		; operate
	jp	c_rtofl2	; copy result and return
;
	end
