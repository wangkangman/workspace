;	LONG MULTIPLY
;	Copyright (c) 2006 by COSMIC Software
;	- 1st arg and result in LONG ACCU
;	- 2nd arg pointed by X
;
	xdef	c_lmul, c_lmulc
	xref.b	c_x, c_lreg
	.dcall	"6,0,c_lmul"
;
c_lmul:
	ld	a,(3,x)		; copy operand
	push	a		; on stack
	ld	a,(2,x)
	push	a
	ldw	x,(x)
	pushw	x
c_lmulc:
	ldw	x,(3,sp)	; level 1
	ld	a,c_lreg
	mul	x,a		; HH * LL
	ldw	c_x,x
	ldw	x,c_lreg
	ld	a,(3,sp)
	mul	x,a		; HL * LH
	addw	x,c_x
	ldw	c_x,x
	ldw	x,c_lreg+1
	ld	a,(2,sp)
	mul	x,a		; LH * HL
	addw	x,c_x
	ldw	c_x,x
	ldw	x,c_lreg+2
	ld	a,(1,sp)
	mul	x,a		; HL * LH
	addw	x,c_x
	ld	a,xl
	ld	c_lreg,a
	ldw	x,c_lreg	; level 2
	ld	a,(4,sp)
	mul	x,a		; HL * LL
	clr	c_lreg+1
	addw	x,c_lreg
	ldw	c_lreg,x
	ldw	x,c_lreg+1
	ld	a,(3,sp)
	mul	x,a		; LH * LH
	addw	x,c_lreg
	ldw	c_lreg,x
	ldw	x,c_lreg+2
	ld	a,(2,sp)
	mul	x,a		; LL * HL
	addw	x,c_lreg
	ldw	c_lreg,x
	ldw	x,c_lreg+1	; level 3
	ld	a,(4,sp)
	mul	x,a		; LH * LL
	clr	c_lreg+2
	addw	x,c_lreg+1
	ldw	c_lreg+1,x
	jrnc	nc1
	inc	c_lreg
nc1:
	ldw	x,c_lreg+2
	ld	a,(3,sp)
	mul	x,a		; LL * LH
	addw	x,c_lreg+1
	ldw	c_lreg+1,x
	jrnc	nc2
	inc	c_lreg
nc2:
	ldw	x,c_lreg+2	; level 4
	ld	a,(4,sp)
	mul	x,a		; LL * LL
	clr	c_lreg+3
	addw	x,c_lreg+2
	ldw	c_lreg+2,x
	jrnc	nc3
	ldw	x,c_lreg
	incw	x
	ldw	c_lreg,x
nc3:
	addw	sp,#4		; clean stack
	ret			; and return
;
	end
