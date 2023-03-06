/*
 * UltraSonic_prog.c
 *
 *  Created on: Mar 1, 2023
 *      Author: LENOVO
 */

#include "avr/delay.h"
#include "Mcu_HW.h"
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "Dio_int.h"
#include "Dio_types.h"

#include "GIE_init.h"
#include "TIMER0_int.h"
#include "TIMER1_init.h"
#include "LCD_Init.h"
#include "LCD_cfg.h"
#include "GIE_init.h"
#include "UtraSonic.cfg.h"
#include "UtraSonic_priv.h"
#include "UtraSonic_types.h"
#include "UtraSonic_init.h"


volatile static u8 Global_edgeCount = 0;
volatile static u16 Global_EchoHighTime = 0 ;

//static void H_Ultrasonic_StaticVoid_Trigger(void);


void H_UltraSonic_Void_init(void)
{
	/*Timer1 Init  */
	M_TIMER1_Void_Init(TIMER1_NORMAL_MODE, TIMER1_NORMAL_WG);

	/* ICU set trigg*/
	M_TIMER1_Void_ICU_setTrigg(ICU_RAISING);

	/* Set prescaller*/
	M_TIMER1_Void_Start(TIMER1_CLK_8);

	/* Enable the Input Capture interrupt to generate an interrupt */
	M_TIMER1_Void_IntEnable(TIMER1_ICU);

	M_TIMER1_Void_Clear_ICU();

	/* Setup the function */
	M_TIMER1_Void_ICU_setCallBack(H_Ultrasonic_Void_edgeProcessing);

	M_GIE_Void_Enable();

}

void H_Ultrasonic_StaticVoid_Trigger()
{
	M_Dio_void_setPinValue(UtraSonic_TRIGG_PIN,DIO_HIGH);
	_delay_us(10);
	M_Dio_void_setPinValue(UtraSonic_TRIGG_PIN,DIO_LOW);
}

u16 H_Ultrasonic_U16_readDistance()
{
	/* Send trigger*/
	H_Ultrasonic_StaticVoid_Trigger();


	return ((0.017*Global_EchoHighTime)/2);
}

void H_Ultrasonic_Void_edgeProcessing()
{

	Global_edgeCount ++ ;

	if (Global_edgeCount == 1)
	{
		/* Clear ICU Value*/
		M_TIMER1_Void_Clear_ICU();

		/*change Trigg to Echo */
		M_TIMER1_Void_ICU_setTrigg(ICU_FALLING);

	}
	else if (Global_edgeCount == 2)
	{
		/*Save the largest Time in Globale variable */
		Global_EchoHighTime = M_TIMER1_U16_GetICU() ;

		/*change to rising */
		M_TIMER1_Void_ICU_setTrigg(ICU_RAISING);

		/* Clear ICU Value*/
		M_TIMER1_Void_Clear_ICU();

		Global_edgeCount = 0 ;

	}
}

