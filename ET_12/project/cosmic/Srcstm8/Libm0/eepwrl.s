;	WRITE A LONG IN EEPROM
;	Copyright (c) 2006 by COSMIC Software
;	- eeprom address in X
;	- value in LONG accu
;
	xdef	c_eewrl
	xref.b	c_x, c_lreg
	include	"eeprom.i"
	.dcall	"2,0,c_eewrl"
;
c_eewrl:
	ldw	y,#c_lreg	; source address
pbcl:
	bset	EEPCR,#E2LAT	; set latch bit
lbcl:
	ld	a,(y)		; get byte
	ld	(x),a		; and store in latch
	incw	y		; next source byte
	incw	x		; next eeprombyte
	ld	a,xl		; test latch boundary
	bcp	a,#$0f
	jreq	prog		; program latch
	cpw	y,#c_lreg+4	; last byte ?
	jrne	lbcl		; loop if more
prog:
	bset	EEPCR,#E2PGM	; set program bit
wait:
	btjt	EEPCR,#E2PGM,wait ; wait for the end
	cpw	y,#c_lreg+4	; last byte ?
	jrne	pbcl		; and loop back
	ret			; return
;
	end
