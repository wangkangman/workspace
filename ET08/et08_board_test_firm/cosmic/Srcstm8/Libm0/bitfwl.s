;	UPDATE LONG BITFIELD IN EXTERNAL MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- address in Y
;	- value in X:A, mask in X extension
;
	xdef	c_bitfwl
	xref.b	c_x, c_y
	.dcall	"2,0,c_bitfwl"
;
c_bitfwl:
	xor	a,(3,y)		; merge value
	and	a,c_x+1		; with mask
	xor	a,(3,y)		; and result
	ld	(3,y),a		; in place
	ld	a,xl		; now MSB
	xor	a,(2,y)		; merge value
	and	a,c_x		; with mask
	xor	a,(2,y)		; and result
	ld	(2,y),a		; in place
	ret			; and return
;
	end
