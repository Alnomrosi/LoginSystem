/*
 * TIMER1_types.h
 *
 *  Created on: Feb 26, 2023
 *      Author: LENOVO
 */

#ifndef TIMER1_TIMER1_TYPES_H_
#define TIMER1_TIMER1_TYPES_H_


typedef enum{

	TIMER1_NORMAL_MODE

}TIMER1_MODE_TYPE;

typedef enum{

	TIMER1_NORMAL_WG

}TIMER1_WG_TYPE;

typedef enum{
	ICU_FALLING,
	ICU_RAISING

}TIMER1_ICU_SELECT;

typedef enum{

	TIMER1_NO_CLK_SOURCE,
	TIMER1_CLK_1,
	TIMER1_CLK_8,
	TIMER1_CLK_64,
	TIMER1_CLK_256,
	TIMER1_CLK_1024


}TIMER1_PRESCALER_SELECT;

typedef enum{
	TIMER1_ICU,
	TIMER1_OCMA,
	TIMER1_OCMB,
	TIMER1_OVF

}TIMER1_INTERRUPT_TYPE;

#endif /* TIMER1_TIMER1_TYPES_H_ */
