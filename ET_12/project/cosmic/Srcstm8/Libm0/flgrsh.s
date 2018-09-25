;	SIGNED LONG SHIFT RIGHT IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg pointed by X and pointer extension
;	- 2nd arg in A
;
	xdef	c_flgrsh
	xref.b	c_x
	.dcall	"4,0,c_flgrsh"
;
c_flgrsh:
	tnz	a		; test zero count
	jreq	fini		; yes, exit
	pushw	y		; save register
	clrw	y
	ld	yl,a		; set counter
	ldw	c_x+1,y		; set address
bcl:
	clrw	x
	ldf	a,([c_x.e],x)	; shift right
	sra	a
	ldf	([c_x.e],x),a
	incw	x
	ldf	a,([c_x.e],x)
	rrc	a
	ldf	([c_x.e],x),a
	incw	x
	ldf	a,([c_x.e],x)
	rrc	a
	ldf	([c_x.e],x),a
	incw	x
	ldf	a,([c_x.e],x)
	rrc	a
	ldf	([c_x.e],x),a
	decw	y		; count down
	jrne	bcl		; and loop back
	ldw	x,c_x+1		; restore address
	popw	y		; restore register
fini:
	ret			; and return
;
	end
