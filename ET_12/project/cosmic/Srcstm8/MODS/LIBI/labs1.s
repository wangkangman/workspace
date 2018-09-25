;	ABSOLUTE VALUE OF A LONG INTEGER
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f_labs
	xref	d_ltor
	xref.b	c_lreg
	xref	d_lneg
	.dcall	"3,4,f_labs"
;
f_labs:
	ldw	x,sp
	addw	x,#4
	callf	d_ltor
	ld	a,c_lreg
	jrpl	pos
	jpf	d_lneg
pos:
	retf	
;
	end
