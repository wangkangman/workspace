;	VERIFY 8 BIT SINGLE CHECKSUM
;	Copyright (c) 2008 by COSMIC Software
;
	xdef	f__checksum, __cktype__
	xref	__ckdesc__
	xref.b	c_x, c_y
	.dcall	"3,0,f__checksum"
;
__cktype__: equ	0		; 8 bit single CRC
;
f__checksum:
	ldw	y,#__ckdesc__	; descriptor address
	clr	a		; crc accumulator
bcld:
	tnz	(y)		; test flag
	jreq	find		; end of list, exit
	ldw	x,y
	ldw	x,(1,x)		; code address
bclc:
	add	a,#$80		; rotate
	rlc	a		; crc
	xor	a,(x)		; accumulate
	incw	x		; next byte
	cpw	x,(3,y)		; check end of block
	jrne	bclc		; no, continue
	addw	y,#5		; next descriptor
	jra	bcld		; and continue
find:
	cpl	a		; invert
	xor	a,(1,y)		; result should be zero
	clrw	x		; for int
	ld	xl,a		; result
	retf			; and return
;
	end
