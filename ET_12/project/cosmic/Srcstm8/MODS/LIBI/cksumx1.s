;	VERIFY 8 BIT MULTIPLE CHECKSUM
;	Copyright (c) 2001 by COSMIC Software
;
	xdef	f__checksumx, __cktype__
	xref	__ckdesc__
	.dcall	"3,0,f__checksumx"
;
__cktype__: equ	2		; 8 bit multiple CRC
;
f__checksumx:
	ldw	y,#__ckdesc__	; descriptor address
bcld:
	ld	a,(y)		; descriptor flag
	jreq	find		; end of list, exit
ok1:
	ldw	x,y
	ldw	x,(1,x)		; code address
	clr	a		; clear CRC
bclc:
	add	a,#$80		; rotate
	rlc	a		; crc
	xor	a,(x)		; accumulate
	incw	x		; next byte
	cpw	x,(3,y)		; check end of block
	jrne	bclc		; no, continue
	cpl	a		; invert CRC
	xor	a,(5,y)		; should be zero
	jrne	find		; no, exit
	addw	y,#6		; skip to next descriptor
	jra	bcld		; and continue
find:
	clrw	x		; complete result
	ld	xl,a
	retf			; and return
;
	end
