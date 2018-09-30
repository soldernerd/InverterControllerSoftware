/* 
 * File:   encoder.h
 * Author: luke
 *
 * Created on 22. Juli 2018, 17:29
 */

#ifndef ENCODER_H
#define	ENCODER_H

#include <xc.h>
#include <stdint.h>

#define INVERSE_DIRECTION

void encoder_init(void);
void timer_init(void);
inline void timer_1_isr(void);
inline void encoder_isr(void);
inline void capture_isr(void);




#endif	/* ENCODER_H */

