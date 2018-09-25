;	UNSIGNED LONG SHIFT RIGHT IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg pointed by X and pointer extension
;	- 2nd arg in A
;
	xdef	d_flgursh
	xref.b	c_x
	.dcall	"5,0,d_flgursh"
;
d_flgursh:
	tnz	a		; test zero count
	jreq	fini		; yes, exit
	pushw	y		; save register
	clrw	y
	ld	yl,a		; set count
	ldw	c_x+1,x		; set address
bcl:
	clrw	x
	ldf	a,([c_x.e],x)	; shift right
	srl	a
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
	retf			; and return
;
	end
