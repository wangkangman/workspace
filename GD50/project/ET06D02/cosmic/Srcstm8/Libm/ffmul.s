;	FLOAT MULTIPLY WITH FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in FLOAT ACCU
;	- second operand pointed by X
;
	xdef	d_ffmul
	xref	d_okmul
	xref.b	c_lreg, c_x
	.dcall	"7,0,d_ffmul"
;
d_ffmul:
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
	push	a
	jpf	d_okmul		; and continue
fini:
	retf
;
	end
