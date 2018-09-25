/* 
 * File:   SPI.h
 * Author: Kimi
 *
 * Created on November 13, 2017, 8:32 AM
 */

#ifndef SPI_H
#define	SPI_H
#include"type.h"

VODET SPI_init(VODET);
VODET SPI_send(U08ET* buf, U08ET length);
VODET  SPI_send_string(U08ET* str, U08ET length);

#endif	/* SPI_H */

