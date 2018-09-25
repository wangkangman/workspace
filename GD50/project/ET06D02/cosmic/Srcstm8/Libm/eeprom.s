;	EEPROM WRITE ROUTINES
;	Copyright (c) 2006 by COSMIC Software
;	- eeprom address in X
;	- value in A (char) or on stack (int)
;
	xdef	d_eewrc, d_eewrw
	xref.b	c_x
	include	"eeprom.i"
	.dcall	"5,0,d_eewrw"
	.dcall	"3,0,d_eewrc"
;
;	program a word
;
d_eewrw:
	ld	a,xl		; get address LSB
	inc	a		; next byte
	bcp	a,#$0f		; test last 4 bits
	jrne	fast		; need one cycles
	ld	a,(4,sp)	; program
	callr	d_eewrc		; MSB
	bset	EEPCR,#E2LAT	; latch address
	jra	last		; program LSB
fast:
	bset	EEPCR,#E2LAT	; latch address
	ld	a,(4,sp)	; store
	ld	(x),a		; MSB
last:
	ld	a,(5,sp)	; store
	ld	(1,x),a		; LSB
	jra	prog		; and program word
;
;	program one byte
;
d_eewrc:
	bset	EEPCR,#E2LAT	; latch address
	ld	(x),a		; store value
prog:
	bset	EEPCR,#E2PGM	; start programming
wait:
	btjt	EEPCR,#E2PGM,wait ; wait for the end
	retf			; and return
;
	end
