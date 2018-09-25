;	FLOAT MULTIPLY WITH FAR MEMORY
;	Copyright (c) 2006 by COSMIC Software
;	- first operand in FLOAT ACCU
;	- second operand pointed by X
;
	xdef	c_ffmul
	xref	c_okmul
	xref.b	c_lreg, c_x
	.dcall	"6,0,c_ffmul"
;
c_ffmul:
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
	jp	c_okmul		; and continue
fini:
	ret
;
	end
