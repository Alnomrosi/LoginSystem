/*
 * TIMER1_init.h
 *
 *  Created on: Feb 26, 2023
 *      Author: LENOVO
 */


#ifndef TIMER1_TIMER1_INIT_H_
#define TIMER1_TIMER1_INIT_H_

#include "STD_TYPES.h"
#include "TIMER1_types.h"


void M_TIMER1_Void_Init(TIMER1_MODE_TYPE Copy_Mode, TIMER1_WG_TYPE Copy_WG_Type);
void M_TIMER1_Void_Start(TIMER1_PRESCALER_SELECT);

void M_TIMER1_Void_IntEnable(TIMER1_INTERRUPT_TYPE Copy_InterruptType);
void M_TIMER1_Void_ICU_IntDisable();
void M_TIMER1_Void_ICU_setTrigg(TIMER1_ICU_SELECT TriggType);
u16 M_TIMER1_Void_ICU_takeReading();
void M_TIMER1_Void_ICU_setCallBack();

u8 M_TIMER1_u8_ChecktFlag(TIMER1_INTERRUPT_TYPE Copy_InterruptType);
void M_TIMER1_Void_ClearFlag(TIMER1_INTERRUPT_TYPE Copy_InterruptType);

void M_TIMER1_Void_Clear_ICU();
u16 M_TIMER1_U16_GetICU();

#endif /* TIMER1_TIMER1_INIT_H_ */
