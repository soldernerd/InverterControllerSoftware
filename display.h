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

void display_init(void);
void display_set(uint16_t value);
inline void display_update();

#endif	/* DISPLAY_H */

