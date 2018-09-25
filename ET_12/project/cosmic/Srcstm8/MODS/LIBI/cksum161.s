;	VERIFY 16 BIT SINGLE CHECKSUM
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	f__checksum16, __cktype__
	xref	__ckdesc__
	xref.b	c_x
	.dcall	"3,0,f__checksum16"
;
__cktype__: equ	4		; 16 bit single CRC
;
	switch	.text
;
f__checksum16:
	ldw	y,#__ckdesc__	; descriptor address
	clr	a		; crc accumulator
	ld	c_x,a
bcld:
	tnz	(y)		; test flag
	jreq	find		; end of list, exit
	ldw	x,y
	ldw	x,(1,x)		; code address
bclc:
	btjt	c_x,#7,here	; get bit 15
here:
	rlc	a		; and rotate
	rlc	c_x		; crc
	xor	a,(x)		; accumulate
	incw	x		; next byte
	cpw	x,(3,y)		; check end of block
	addw	y,#5		; skip to next descriptor
	jra	bcld		; and continue
find:
	cpl	a		; invert
	xor	a,(2,y)
	ld	xl,a
	ld	a,c_x		; access high byte
	cpl	a		; invert
	xor	a,(1,y)		; result should be zero
	ld	xh,a
	retf			; and return
;
	end
