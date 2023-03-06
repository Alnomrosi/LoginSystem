/*
 * KEYPAD_prog.c
 *
 *  Created on: Feb 22, 2023
 *      Author: LENOVO
 */

/******************************************
  INCLUDES
*******************************************/
#include <util/delay.h>
#include "STD_TYPES.h"
#include "Dio_types.h"
#include "Dio_int.h"
#include "LCD_Init.h"

#include "KEYPAD_cfg.h"
#include "KEYPAD_priv.h"
#include "KEYPAD_types.h"
#include "KEYPAD_Init.h"


/******************************************
  Local Data (static global )
*******************************************/



/******************************************
  Global Data (non static global)
*******************************************/

//static s32 KEYPAD_KEYS_NUMBERS[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
static u8 KEYPAD_KEYS_CALC[4][4] = {{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'A','0','=','+'}};

u8 ARR_ROWS[4] = {KEYPAD_R0_PIN,KEYPAD_R1_PIN,KEYPAD_R2_PIN,KEYPAD_R3_PIN};
u8 ARR_COL[4] = {KEYPAD_C0_PIN,KEYPAD_C1_PIN,KEYPAD_C2_PIN,KEYPAD_C3_PIN};

/******************************************
  Local Functions body
*******************************************/


/******************************************
  Global Functions body
*******************************************/

/******************************************
  syntax : void H_LCD_void_creatCustomChar(const u8 * ArrPattern,u8 copy_u8charCode)


  Description : Create Custom character

  Synch/Asynch     : Synchronouse
  Reentrance       : Reentrant
  parameters (in)  :  copy_s32Num
  parameters (out) :  Non
  Return Value     :  Non
******************************************/

char H_KEYPAD_u8_getPressedKey(void)
{
	u8 Local_ROW_Counter = 0 ;
	u8 Local_COL_Counter = 0 ;
	u8 Local_Return_KEY  = '0' ;

	for(Local_ROW_Counter = 0; Local_ROW_Counter <= 3; Local_ROW_Counter++ )
	{
		/*Set All ROW PINS to high */
			M_Dio_void_setPinValue(ARR_ROWS[0],DIO_HIGH);
			M_Dio_void_setPinValue(ARR_ROWS[1],DIO_HIGH);
			M_Dio_void_setPinValue(ARR_ROWS[2],DIO_HIGH);
			M_Dio_void_setPinValue(ARR_ROWS[3],DIO_HIGH);

		/*Set row pin to check each row */
		M_Dio_void_setPinValue(ARR_ROWS[Local_ROW_Counter],DIO_LOW);

		for(Local_COL_Counter = 0; Local_COL_Counter<= 3; Local_COL_Counter++ )
			{
			/*check if the COL is LOW then button pressed */
				if (M_Dio_en_getPinValue(ARR_COL[Local_COL_Counter]) == DIO_LOW)
					{

			/*delay for bouncing and busy wait till button is released */

				while(M_Dio_en_getPinValue(ARR_COL[Local_COL_Counter]) == DIO_LOW);
				_delay_ms(50);
				Local_Return_KEY = KEYPAD_KEYS_CALC[Local_ROW_Counter][Local_COL_Counter] ;
				return Local_Return_KEY ;
					}
			}

	}


		return NOT_PRESSED ;
	}

