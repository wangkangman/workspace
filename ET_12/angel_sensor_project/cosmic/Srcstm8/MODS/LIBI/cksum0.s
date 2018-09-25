;	VERIFY 8 BIT SINGLE CHECKSUM
;	Copyright (c) 2006 by COSMIC Software
;
	xdef	__checksum, __cktype__
	xref	__ckdesc__
	.dcall	"2,0,__checksum"
;
__cktype__: equ	0		; 8 bit single CRC
;
__checksum:
	ldw	y,#__ckdesc__	; descriptor address
	clr	a		; crc accumulator
bcld:
	tnz	(y)		; test flag
	jreq	find		; end of list, exit
	ldw	x,y		; load
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
	ret			; and return
;
	end
