;	FLOAT DIVIDE WITH FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in FLOAT ACCU
;	- second operand pointed by X
;
	xdef	d_ffdiv
	xref	d_okdiv
	xref.b	c_lreg, c_x
	.dcall	"9,0,d_ffdiv"
;
d_ffdiv:
	ld	a,c_lreg	; test zero
	or	a,c_lreg+1
	jreq	fini		; yes, exit
	ldw	c_x+1,x		; set address
	ldw	x,#3
	ldf	a,([c_x.e],x)	; load operand
	push	a
	decw	x
	ldf	a,([c_x.e],x)
	push	a
	decw	x
	ldf	a,([c_x.e],x)
	push	a
	ldf	a,[c_x.e]
	jpf	d_okdiv		; and continue
fini:
	retf			; and return
;
	end
