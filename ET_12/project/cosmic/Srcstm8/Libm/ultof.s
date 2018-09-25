;	UNSIGNED LONG INTEGER TO FLOAT CONVERSION
;	Copyright (c) 2006 by COSMIC Software
;	- operand and result in ACCU
;
	xdef	d_ultof
	xref.b	c_lreg
	.dcall	"7,0,d_ultof"
;
d_ultof:
	pushw	x		; save
	pushw	y		; registers
	ld	a,#23		; start exponent
	tnz	c_lreg		; check MSB
	jrne	panul		; not nul, continue
	ldw	x,c_lreg	; test zero
	jrne	fasta
	ldw	y,c_lreg+2
	jreq	fini		; zero, exit
fasta:
	tnz	c_lreg+1	; check MSB
	jrmi	panor		; normalize ok
	jrne	bcla		; not nul, slow shift
	mov	c_lreg+1,c_lreg+2	; byte shift
	mov	c_lreg+2,c_lreg+3
	clr	c_lreg+3
	sub	a,#8		; update exponent
	jra	fasta
bcla:
	dec	a		; update exponent
	sllw	y		; shift value
	rlc	c_lreg+1
	jrpl	bcla		; and loop again
	jra	panor		; and continue
panul:
	ldw	x,c_lreg
	ldw	y,c_lreg+2
bclb:
	inc	a		; update exponent
	srlw	x		; shift value
	rrcw	y
	srl	c_lreg		; if MSB not nul
	jrne	bclb		; loop again
	exg	a,xl
	ld	c_lreg+1,a
	exg	a,xl
panor:
	sll	c_lreg+1	; remove hidden bit
	add	a,#127		; add offset
	srl	a		; exponent ok
	rrc	c_lreg+1	; align mantissa
	ld	c_lreg,a	; store result
fini:
	popw	y		; restore
	popw	x		; registers
	retf			; and return
;
	end
