;	SWITCH WITH RELATIVE OFFSET
;	Copyright (c) 2007 by COSMIC Software
;	- table address after the call
;	- value in A or X
;
	xdef	d_jctab, d_jstab
	xref.b	c_x
;
d_jctab:
	clrw	x		; set index
	ld	xl,a
d_jstab:
	sllw	x		; word index
	pop	c_x		; get
	pop	c_x+1		; table
	pop	c_x+2		; address
	ldf	a,([c_x.e],x)	; get
	ld	yh,a		; offset
	incw	x
	ldf	a,([c_x.e],x)
	ld	yl,a
	clr	a		; prepare
	exgw	x,y		; sign
	tnzw	x		; extension
	jrpl	pos
	dec	a		; negative
pos:
	addw	x,c_x+1		; compute
	adc	a,c_x		; target
	ldw	c_x+1,x		; in
	ld	c_x,a		; place
	jpf	[c_x.e]		; and go
;
	end
