;	VERIFY 16 BIT MULTIPLE CHECKSUM
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	__checksum16x, __cktype__
	xref	__ckdesc__
	xref.b	c_x
	.dcall	"2,0,__checksum16x"
;
__cktype__: equ	6		; 16 bit multiple CRC
;
	switch	.text
;
__checksum16x:
	ldw	y,#__ckdesc__	; descriptor address
bcld:
	ld	a,(y)		; descriptor flag
	jreq	find		; end of list, exit
ok1:
	ldw	x,y
	ldw	x,(1,x)		; code address
	clr	a		; clear CRC
	ld	c_x,a
bclc:
	btjt	c_x,#7,here	; get bit 15
here:
	rlc	a		; and rotate
	rlc	c_x		; crc
	xor	a,(x)		; accumulate
	incw	x		; next byte
	cpw	x,(3,y)		; check end of block
	jrne	bclc		; no, continue
	cpl	a		; invert CRC
	xor	a,(6,y)		; should be zero
	jrne	find		; no, exit
	ld	a,c_x		; check high byte
	cpl	a		; invert
	xor	a,(5,y)		; should be zero
	jrne	find		; no, exit
	addw	y,#7		; skip to next descriptor
	jra	bcld		; and continue
find:
	clrw	x		; complete result
	ld	xl,a
	ret			; and return
;
	end
