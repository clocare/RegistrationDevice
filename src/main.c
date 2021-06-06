/*
 * main.c
 *
 *  Created on: Aug 15, 2020
 *      Author: user
 */

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "SPI1.h"

#include "mfrc522.h"
#include "Reader.h"


//#include "FPEC_interface.h"
//#include "UART_interface.h"
//#include "LCD.h"
//#include "IWDG_interface.h"
//u32 result1 ;
//u32 result2 ;
//u8 arr[]={0x14,0x00,0x10,0x00,0x01,0x02,0x03,0x04};
//u8 status ;
boolean x=FALSE;
u8  i[23]={0};
int main()
{
//u8  i[23]={0};

	RCC_voidInitSysClock();

	RCC_voidEnableClockPrephiral(RCC_APB2 ,  12);
	RCC_voidEnableClockPrephiral(RCC_APB2, 0);
	RCC_voidEnableClockPrephiral(RCC_APB1, 1);


	MGPIO_VidSetPinDirection(GPIOB, PIN12, 0b0001);	// Chip Select  PIN
	MGPIO_VidSetPinDirection(GPIOB, PIN13, 0b1001);
	MGPIO_VidSetPinDirection(GPIOB, PIN14, INPUT_FLOATING);					// MISO PIN - Not IN USE
	MGPIO_VidSetPinDirection(GPIOB, PIN15, 0b1001);
	SPI1_voidInit();
	Reader_init();
	//UART1_voidInit();
	/*Enable RCC for crc*/
	//RCC_voidEnableClockPrephiral(RCC_AHB , 6);
	/*Enable RCC for GPIOB*/
	//RCC_voidEnableClockPrephiral(RCC_APB2 , 2);
	//RCC_voidEnableClockPrephiral(RCC_AHB , 4);
	//RCC_voidEnableClockPrephiral(RCC_APB2 , 14);
	//MGPIO_VidSetPinDirection( GPIOA ,PIN9 ,0b1001);
	//MGPIO_VidSetPinDirection( GPIOA ,PIN10 ,0b0100);
	 loginData_Type patient;
	 loginData_Type patient1;
	 patient.id_w[ID_WRITE_LEN]=0x10;
	 patient.id_r[ID_READ_LEN]=0x00;
	 patient.Pass_w[ID_READ_LEN]=0x11;
	 patient.Pass_r[ID_READ_LEN]=0x00;
	 x=Reader_isNewCardPresent();
if(x==TRUE){
	x = Reader_SetLoginData(patient);

}
else{
	return 0;
}
	//LCD_hand_shake(i);


	return 0 ;
}




