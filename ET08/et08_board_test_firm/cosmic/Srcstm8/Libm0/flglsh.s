;	LONG SHIFT LEFT IN FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg pointed by X and pointer extension
;	- 2nd arg in A
;
	xdef	c_flglsh
	xref.b	c_x
	.dcall	"4,0,c_flglsh"
;
c_flglsh:
	tnz	a		; test zero count
	jreq	fini		; yes, exit
	pushw	y		; save register
	clrw	y
	ld	yl,a		; set count
	ldw	c_x+1,x		; set address
bcl:
	ldw	x,#3
	ldf	a,([c_x.e],x)	; shift left
	sll	a
	ldf	([c_x.e],x),a
	decw	x
	ldf	a,([c_x.e],x)
	rlc	a
	ldf	([c_x.e],x),a
	decw	x
	ldf	a,([c_x.e],x)
	rlc	a
	ldf	([c_x.e],x),a
	ldf	a,[c_x.e]
	rlc	a
	ldf	[c_x.e],a
	decw	y		; count down
	jrne	bcl		; and loop back
	ldw	x,c_x+1		; restore address
	popw	y		; restore register
fini:
	ret			; and return
;
	end
