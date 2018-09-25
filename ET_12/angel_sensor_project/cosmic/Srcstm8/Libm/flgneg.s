;	LONG NEGATE IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- arg and result pointed by X and extension
;
	xdef	d_flgneg
	xref.b	c_x
	.dcall	"3,0,d_flgneg"
;
d_flgneg:
	ldw	c_x+1,x		; set address
	clrw	x
	ldf	a,([c_x.e],x)
	cpl	a		; invert
	ldf	([c_x.e],x),a
	incw	x
	ldf	a,([c_x.e],x)
	cpl	a
	ldf	([c_x.e],x),a
	incw	x
	ldf	a,([c_x.e],x)
	cpl	a
	ldf	([c_x.e],x),a
	incw	x
	ldf	a,([c_x.e],x)
	neg	a
	ldf	([c_x.e],x),a
	jrne	fini
	decw	x
	ldf	a,([c_x.e],x)	; propagate carry
	inc	a
	ldf	([c_x.e],x),a
	jrne	fini
	decw	x
	ldf	a,([c_x.e],x)
	inc	a
	ldf	([c_x.e],x),a
	jrne	fini
	decw	x
	ldf	a,([c_x.e],x)
	inc	a
	ldf	([c_x.e],x),a
fini:
	ldw	x,c_x+1		; restore register
	retf			; and return
;
	end
