;	SIGNED LONG SHIFT RIGHT IN MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg pointed by X
;	- 2nd arg in A
;
	xdef	c_lgrsh
	xref.b	c_lreg
	.dcall	"2,0,c_lgrsh"
;
c_lgrsh:
	tnz	a		; test zero count
	jreq	fini		; yes, exit
bcl:
	sra	(x)		; shift right
	rrc	(1,x)
	rrc	(2,x)
	rrc	(3,x)
	dec	a		; count down
	jrne	bcl		; and loop back
fini:
	ret			; and return
;
	end
