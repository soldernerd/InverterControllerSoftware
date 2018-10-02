/* 
 * File:   display.h
 * Author: luke
 *
 * Created on 21. Juli 2018, 23:18
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#include <xc.h>
#include <stdint.h>

#define DECIMAL_0 0b00000001
#define DECIMAL_1 0b00000010
#define DECIMAL_2 0b00000100
#define DECIMAL_3 0b00001000

void display_init(void);
void display_set(int16_t value, uint8_t decimal_flags);
inline void display_isr(void);
void display_startup(void);

#endif	/* DISPLAY_H */

