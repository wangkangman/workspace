;	WRITE A STRUCTURE IN EEPROM
;	Copyright (c) 2006 by COSMIC Software
;	- eeprom address in X
;	- structure address in Y
;	- structure size in A
;
	xdef	d_eewstr
	xref.b	c_x
	include	"eeprom.i"
	.dcall	"3,0,d_eewstr"
;
d_eewstr:
	ld	c_x,a		; copy count
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
	dec	c_x
	jrne	lbcl		; loop if more bytes
	inc	c_x		; set back
prog:
	bset	EEPCR,#E2PGM	; set program bit
wait:
	btjt	EEPCR,#E2PGM,wait ; wait for the end
	dec	c_x		; count down
	jrne	pbcl		; and loop back
	retf			; return
;
	end
