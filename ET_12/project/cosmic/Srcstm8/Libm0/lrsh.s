;	SIGNED LONG SHIFT RIGHT
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg in A
;
	xdef	c_lrsh
	xref.b	c_lreg
	.dcall	"2,0,c_lrsh"
;
c_lrsh:
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
	ret			; and return
;
	end
