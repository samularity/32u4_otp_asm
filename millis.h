/*
 * millis.h
 *
 * Created: 17.12.2013 19:24:02
 *  Author: Samuel
 */ 
#ifndef MILLIS_H_
#define MILLIS_H_

#include <avr/interrupt.h>

uint32_t millis(void);
void timer_init(void);

#endif /* MILLIS_H_ */