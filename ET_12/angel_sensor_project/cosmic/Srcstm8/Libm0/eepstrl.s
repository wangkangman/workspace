;	WRITE A LARGE STRUCTURE IN EEPROM
;	Copyright (c) 2007 by COSMIC Software
;	- eeprom address in X extension
;	- structure address in Y
;	- structure size in X
;
	xdef	c_eewstrl
	xref.b	c_x, c_y
	include	"eeprom.i"
	.dcall	"2,0,c_eewstrl"
;
c_eewstrl:
	addw	x,c_x		; end address
	ldw	c_y,x		; in memory
	ldw	x,c_x		; eeprom address
pbcl:
	bset	EEPCR,#E2LAT	; set latch bit
lbcl:
	ld	a,(y)		; get byte
	ld	(x),a		; and store in latch
	incw	y		; next byte
	incw	x		; next byte
	ld	a,xl		; test latch boundary
	bcp	a,#$0f
	jreq	prog		; program latch
	cpw	x,c_y
	jrne	lbcl		; loop if more bytes
prog:
	bset	EEPCR,#E2PGM	; set program bit
wait:
	btjt	EEPCR,#E2PGM,wait ; wait for the end
	cpw	x,c_y		; check last address
	jrne	pbcl		; and loop back
	ret			; return
;
	end
