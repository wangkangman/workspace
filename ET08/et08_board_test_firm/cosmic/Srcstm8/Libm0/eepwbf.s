;	WRITE A BITFIELD IN EEPROM
;	Copyright (c) 2006 by COSMIC Software
;	- eeprom address in X extension
;	- value in X
;	- mask in A
;
	xdef	c_eewbf, c_eewbb
	xref	c_eewrc
	xref.b	c_x, c_y
	.dcall	"5,0,c_eewbf"
	.dcall	"4,0,c_eewbb"
;
c_eewbf:
	push	a		; save mask
	cpl	a		; invert it
	ldw	c_y,x		; copy value
	and	a,c_y+1		; prepare new value
	ld	c_y,a		; and keep it
	pop	a		; get mask
	and	a,[c_x.w]	; and with bitfield
	or	a,c_y		; insert value
c_eewbb:
	ldw	x,c_x		; restore address
	call	c_eewrc		; program byte and return
	incw	x		; prepare
	ldw	c_x,x		; byte
	ret			; and return
;
	end
