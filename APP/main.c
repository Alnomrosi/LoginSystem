/*
 * main.c
 *
 *  Created on: Mar 4, 2023
 *      Author: LENOVO
 */
/********************************************************************
 *	Includes
 *******************************************************************/
#include <avr/delay.h>
#include "STD_TYPES.h"

#include "Port_cfg.h"
#include "Port_priv.h"
#include "Port_types.h"
#include "Port_int.h"

#include "Dio_int.h"

#include "LED_CFG.h"
#include "LED_init.h"
#include "LCD_cfg.h"
#include "LCD_Init.h"

#include "KEYPAD_cfg.h"
#include "KEYPAD_Init.h"

#include "UtraSonic_types.h"
#include "UtraSonic_init.h"

#include "GIE_init.h"
#include "TIMER0_int.h"
#include "TIMER0_priv.h"

/********************************************************************
 *	Functions
 *******************************************************************/
void SavePassword(u8 *Copy_Arr);
u8 CheckPassword(u8 *Copy_Arr);
void FlagCheck_Func();

/********************************************************************
 *	Global Variables
 *******************************************************************/
#define DataBase_SIZE	4
#define NoOfTrials		2
/* Configuration array of the PORT*/
extern const Port_ConfigType PORT_Config_Arr[PORT_PINS_CONFIG_ARR_SIZE];

u8 GlobalDataBase_Password[4] = {'1','2','3','4'} ;
u8 GlobalFlag = 0 ;

void main()
{
/********************************************************************
 * 	Initiations Sections
 *	 Init Name		  	 Available Arguments:
 *     PORT			  		 Config Array
 *     LCD			     		 Void
 *     UltraSonic			     Void
 *     TIMER0			NORMAL MODE, NORMAL PORT
 *******************************************************************/
	Port_Init(PORT_Config_Arr);
	H_LCD_void_Init();
	H_UltraSonic_Void_init();
	M_TIMER0_Void_Init(NORMAL_MODE,WG_NORMAL_PORT);
	M_TIMER0_setCallBack(OVRER_FLOW, FlagCheck_Func);
	M_TIMER0_EnableInt(OVRER_FLOW);
/********************************************************************
 *	Local Variables
 *******************************************************************/
	u16 UltraSonicValue = 0 ;
	u8 KeyPadValue = 0;
	u8 PasswordArr[100] = {};
	u8 CheckCounter = 0 ;

/********************************************************************
 *						Start code
 *******************************************************************/
	while(1)
	{
		H_LCD_void_gotoXY(0,0);
		H_LCD_void_sendString("Welcome To Login");
		H_LCD_void_gotoXY(1,5);
		H_LCD_void_sendString("System");

		/* Read UltraSonic Value */
		UltraSonicValue = H_Ultrasonic_U16_readDistance();


		/* Check if Ultrasonic value is less than 15*/
		if ((u8)UltraSonicValue <= 15 && CheckCounter > 5)
		{
		/* Clear LCD */
		H_LCD_void_sendCommand(LCD_CLEAR_DISP);

		/* Show to lcd: Enter paassword */
		CheckCounter = 0 ;
		while(CheckCounter <= NoOfTrials)
		{
			H_LCD_void_sendString("Enter Password: ");
			/* Save entered password to array*/
			H_LCD_void_gotoXY(1,0);
			SavePassword(PasswordArr);
			H_LCD_void_gotoXY(0,0);

			/* Check if the password is correct from Data Base */
			if (CheckPassword(PasswordArr) == 1)
			{
				H_LCD_void_sendString("Correct");
				_delay_ms(1500);
				break ;
			}
			/* IF enter wrong password*/
			else
			{
				H_LCD_void_sendString("Not Correct");
				H_LCD_void_gotoXY(1,0);
				_delay_ms(1500);
				CheckCounter++;
				H_LCD_void_sendCommand(LCD_CLEAR_DISP);

			}
		}

		H_LCD_void_sendCommand(LCD_CLEAR_DISP);
		/* After checking */
		if (CheckCounter > NoOfTrials)
		{
			M_GIE_Void_Enable();
			H_LCD_void_sendString("Wait for 2 min");
			M_TIMER0_setDelayTimeMilliSec(120,NORMAL_MODE,CLK_1024);

			while(GlobalFlag == 0);

			H_LCD_void_sendCommand(LCD_CLEAR_DISP);
			M_GIE_Void_Disable();
		}
		else
		{
			/* Correct Password*/
			H_LCD_void_sendString("Welcome User: ");
			M_Dio_void_setPinValue(DIO_PB7, DIO_HIGH);
			_delay_ms(1500);

			H_LCD_void_sendCommand(LCD_CLEAR_DISP);

			/* LCD GUI*/
			H_LCD_void_gotoXY(0,0);
			H_LCD_void_sendString("Chose:1- LED ON ");
			H_LCD_void_gotoXY(1,6);
			H_LCD_void_sendString("2- LED OFF");
			/*Chose to Turn on or off led*/
			KeyPadValue = 0 ;
			do{
				KeyPadValue = H_KEYPAD_u8_getPressedKey();

			}while(KeyPadValue == NOT_PRESSED);

				H_LCD_void_sendCommand(LCD_CLEAR_DISP);
			if (KeyPadValue == '1' )
			{
				//M_Dio_void_setPinValue(DIO_PA5, DIO_HIGH);
				H_LED_void_Turn_on(LED_BLUE);
				H_LCD_void_gotoXY(0,0);
				H_LCD_void_sendString("LED IS ON ");
			}
			else if (KeyPadValue == '2')
			{
				H_LED_void_Turn_off(LED_BLUE);
				H_LCD_void_gotoXY(0,0);
				H_LCD_void_sendString("LED IS OFF ");
			}
			else
			{
				H_LCD_void_gotoXY(0,0);
				H_LCD_void_sendString("Wrong Choice ");
			}
			_delay_ms(2000);


		}
		}

	CheckCounter ++ ;
	}

	/* Clear Display*/
	H_LCD_void_sendCommand(LCD_CLEAR_DISP);
	/*REST FLAG  */
	GlobalFlag = 0 ;
	/*REST RELAY */
	M_Dio_void_setPinValue(DIO_PB7, DIO_LOW);

}

/********************************************************************
 *						END code
 *******************************************************************/


/********************************************************************
 *					Save Password Function
 *******************************************************************/

void SavePassword(u8 *Copy_Arr)
{
	u8 KeyPadValue = 0;
	u8 Local_Counter = 0 ;

	do{
		KeyPadValue = H_KEYPAD_u8_getPressedKey();
		if (KeyPadValue != NOT_PRESSED )
		{

			/* Save Value to the array */
			Copy_Arr[Local_Counter] = KeyPadValue ;
			H_LCD_void_sendData('*');

			if (Copy_Arr[Local_Counter] == 'A')
			{
				H_LCD_void_sendCommand(LCD_CLEAR_DISP);
				break;
			}

			Local_Counter ++ ;

		}
	}while(KeyPadValue == NOT_PRESSED || Copy_Arr[Local_Counter] != 'N' );

}


/********************************************************************
 *					Check Password Function
 *******************************************************************/
u8 CheckPassword(u8 *Copy_Arr)
{

	u8 Local_Counter = 0;
/*
	while(Copy_Arr[Local_Counter] != 'A')
	{
		H_LCD_void_sendData(Copy_Arr[Local_Counter]);
		Local_Counter ++ ;
	}
	*/
	/*Check if the number is equal the data base */

	for(Local_Counter = 0; Local_Counter <= 3; Local_Counter++)
	{
		if (Copy_Arr[Local_Counter] != GlobalDataBase_Password[Local_Counter])
		{
			return 0 ;
		}
	}
	return 1 ;
}

/********************************************************************
 *					Check Password Function
 *******************************************************************/

void FlagCheck_Func()
{
	GlobalFlag = 1 ;
}
