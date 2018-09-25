;	WRITE A BITFIELD IN EEPROM
;	Copyright (c) 2006 by COSMIC Software
;	- eeprom address in X extension
;	- value in X
;	- mask in A
;
	xdef	d_eewbf, d_eewbb
	xref	d_eewrc
	xref.b	c_x, c_y
	.dcall	"7,0,d_eewbf"
	.dcall	"6,0,d_eewbb"
;
d_eewbf:
	push	a		; save mask
	cpl	a		; invert it
	ldw	c_y,x		; copy value
	and	a,c_y+1		; prepare new value
	ld	c_y,a		; and keep it
	pop	a		; get mask
	and	a,[c_x.w]	; and with bitfield
	or	a,c_y		; insert value
d_eewbb:
	ldw	x,c_x		; restore address
	callf	d_eewrc		; program byte and return
	incw	x		; prepare
	ldw	c_x,x		; byte
	retf			; and return
;
	end
