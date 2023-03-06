/*
 * TIMER1_prog.c
 *
 *  Created on: Feb 26, 2023
 *      Author: LENOVO
 */

#include "avr/interrupt.h"
#include "Mcu_HW.h"
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "TIMER1_init.h"
#include "TIMER1_types.h"
#include "TIMER1_priv.h"
#include "TIMER1_cfg.h"
#include "LCD_Init.h"

void (*ICU_CallBackFun)(void) = NULL ;


void M_TIMER1_Void_Init(TIMER1_MODE_TYPE Copy_Mode, TIMER1_WG_TYPE Copy_WG_Type)
{
	/* Clear Modes Bit	*/
	TIMER1_TCCR1A &= TIMER1_CLEAR_MODE ;
	/* Set the Mode type*/
	if (Copy_Mode == TIMER1_NORMAL_MODE)
	{
		TIMER1_TCCR1A |= TIMER1_NORMAL_MODE_MASK ;

		if ( Copy_WG_Type == TIMER1_NORMAL_WG )
		{
			/*Set the Wave Generation */
			TIMER1_TCCR1A |= Copy_WG_Type;
			(TIMER1_TCCR1B)|= (Copy_WG_Type << TIMER1_SET_BIT_WG);
		}
	}
}

void M_TIMER1_Void_Start(TIMER1_PRESCALER_SELECT Copy_Prescaller)
{

	/* Clear TCCR1B Prescaller */
	TIMER1_TCCR1B &= TIMER1_CLEAR_PRESCALER ;
	/* set TCCR1B the required Prescaller */
	TIMER1_TCCR1B |= Copy_Prescaller ;

}



void M_TIMER1_Void_IntEnable(TIMER1_INTERRUPT_TYPE Copy_InterruptType)
{
	switch(Copy_InterruptType)
	{
	case TIMER1_ICU:
		SET_BIT(TIMER1_TIMSK,TIMSK_TICIE1);
		break;

	case TIMER1_OVF:
		SET_BIT(TIMER1_TIMSK,TIMSK_TOIE1);
		break;
	case TIMER1_OCMA:
		break;
	case TIMER1_OCMB:
		break;
	}
}


void M_TIMER1_Void_ICU_IntDisable()
{
	CLR_BIT(TIMER1_TIMSK,TIMSK_TICIE1);
}


void M_TIMER1_Void_ICU_setTrigg(TIMER1_ICU_SELECT TriggType)
{
		if (TriggType == ICU_FALLING)
		{
				CLR_BIT(TIMER1_TCCR1B, TCCR1B_ICES1);
		}
		else if (TriggType == ICU_RAISING)
		{
				SET_BIT(TIMER1_TCCR1B, TCCR1B_ICES1);
		}
}

u16 M_TIMER1_Void_ICU_takeReading()
{

	u16 Local_ReturnValue = 0 ;

	/*Save the first 8 bit */
	Local_ReturnValue = TIMER1_ICR1 ;

	/*Save the seconed 8 bit */
	Local_ReturnValue = Local_ReturnValue | (TIMER1_ICR1<<8) ;

	return Local_ReturnValue ;

}

u8 M_TIMER1_u8_ChecktFlag(TIMER1_INTERRUPT_TYPE Copy_InterruptType)
{

	switch(Copy_InterruptType)
	{
		case TIMER1_ICU:
				if(GET_BIT(TIMER1_TIFR,TIFR_ICF1) == 1)
				{

					return 1 ;
				}
			break;

		case TIMER1_OVF:
			if(GET_BIT(TIMER1_TIFR,TIFR_TOV0) == 1)
			{
				return 1 ;
			}
			break;
		case TIMER1_OCMA:
			if(GET_BIT(TIMER1_TIFR,TIFR_OCF1A) == 1)
			{
				return 1 ;
			}
			break;
		case TIMER1_OCMB:
			if(GET_BIT(TIMER1_TIFR,TIFR_OCF1B) == 1)
			{
				return 1 ;
			}
			break;
	}

	return 0 ;
}

void M_TIMER1_Void_ClearFlag(TIMER1_INTERRUPT_TYPE Copy_InterruptType)
{

	switch(Copy_InterruptType)
	{
case TIMER1_ICU:
	SET_BIT(TIMER1_TIFR,TIFR_ICF1);
	break;
case TIMER1_OVF:
		SET_BIT(TIMER1_TIFR,TIFR_TOV0);
	break;
case TIMER1_OCMA:
	SET_BIT(TIMER1_TIFR,TIFR_OCF1A);
	break;
case TIMER1_OCMB:
	SET_BIT(TIMER1_TIFR,TIFR_OCF1B);
	break;
	}

}



void M_TIMER1_Void_ICU_setCallBack(void (*Copy_Func)(void))
{
	if (Copy_Func != NULL)
	{
		ICU_CallBackFun = Copy_Func;
	}
}

void M_TIMER1_Void_Clear_ICU()
{
	TIMER1_TCNT1 = 0 ;
	TIMER1_ICR1 = 0 ;
}

u16 M_TIMER1_U16_GetICU()
{

	return TIMER1_ICR1 ;
}



ISR(TIMER1_CAPT_vect)
{
	if ( ICU_CallBackFun != NULL)
	{
	ICU_CallBackFun();
	}
}


ISR(TIMER1_OVF_vect)
{

}
