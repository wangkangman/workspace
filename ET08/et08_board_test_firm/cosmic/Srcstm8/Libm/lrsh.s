;	SIGNED LONG SHIFT RIGHT
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg in A
;
	xdef	d_lrsh
	xref.b	c_lreg
	.dcall	"3,0,d_lrsh"
;
d_lrsh:
	tnz	a		; test zero count
	jreq	fini		; yes, exit
bcl:
	sra	c_lreg		; slow shift right
	rrc	c_lreg+1
	rrc	c_lreg+2
	rrc	c_lreg+3
	dec	a		; count down
	jrne	bcl		; and loop back
fini:
	retf			; and return
;
	end
