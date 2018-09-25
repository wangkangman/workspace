;	UPDATE BITFIELD IN EXTERNAL MEMORY
;	Copyright (c) 2001 by COSMIC Software
;	- address in Y
;	- value in X:A, mask in X extension
;
	xdef	d_bitfw
	xref.b	c_x, c_y
	.dcall	"3,0,d_bitfw"
;
d_bitfw:
	xor	a,(1,y)		; merge value
	and	a,c_x+1		; with mask
	xor	a,(1,y)		; and result
	ld	(1,y),a		; in place
	ld	a,xl		; now MSB
	xor	a,(y)		; merge value
	and	a,c_x		; with mask
	xor	a,(y)		; and result
	ld	(y),a		; in place
	retf			; and return
;
	end
