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

void display_set(uint16_t value);
inline void display_update(uint8_t segment);

#endif	/* DISPLAY_H */

