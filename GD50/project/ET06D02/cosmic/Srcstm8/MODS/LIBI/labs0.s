;	ABSOLUTE VALUE OF A LONG INTEGER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	_labs
	xref	c_ltor
	xref.b	c_lreg
	xref	c_lneg
	.dcall	"2,4,_labs"
;
_labs:
	ldw	x,sp
	addw	x,#3
	call	c_ltor
	ld	a,c_lreg
	jrpl	pos
	jp	c_lneg
pos:
	ret	
;
	end
