/*	INTERRUPT VECTORS TABLE FOR STM8
 *	Copyright (c) 2007 by COSMIC Software
 */
/*	should be the name of a dummy interrupt routine
 *	in order to catch properly unexpected interrupts
 */
#define NULL 0

extern void _stext();		/* startup routine */
extern void recept();		/* character receive handler */

#pragma section const {vector}

void (* const @vector _vectab[32])() = {
	_stext,			/* RESET      */
	NULL,			/* TRAP       */
	NULL,			/* TLI        */
	NULL,			/* AWU        */
	NULL,			/* CLK        */
	NULL,			/* EXTI PORTA */
	NULL,			/* EXTI PORTB */
	NULL,			/* EXTI PORTC */
	NULL,			/* EXTI PORTD */
	NULL,			/* EXTI PORTE */
	NULL,			/* CAN RX     */
	NULL,			/* CAN TX     */
	NULL,			/* SPI        */
	NULL,			/* TIM1 OVF   */
	NULL,			/* TIM1 CAP   */
	NULL,			/* TIM2 OVF   */
	NULL,			/* TIM2 CAP   */
	NULL,			/* TIM3 OVF   */
	NULL,			/* TIM3 CAP   */
	NULL,			/* USART TX   */
	recept,			/* USART RX   */
	NULL,			/* I2C        */
	NULL,			/* LINUART TX */
	NULL,			/* LINUART RX */
	NULL,			/* ADC        */
	NULL,			/* TIM4 OVF   */
	NULL,			/* EEPROM ECC */
	};
