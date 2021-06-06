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
#include "hw_reg.h"

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
	// port a clock
	RCC_voidEnableClockPrephiral(RCC_APB2,2);
	//RCC_voidEnableClockPrephiral(RCC_APB2, 1);
//spi clock
	RCC_voidEnableClockPrephiral(RCC_APB2 ,  12);
	// af func
	RCC_voidEnableClockPrephiral(RCC_APB2, 0);
	// timer clock
	RCC_voidEnableClockPrephiral(RCC_APB1, 1);

	MGPIO_VidSetPinDirection(GPIOA, PIN3 , 0b0001);
	MGPIO_VidSetPinDirection(GPIOA, PIN4, 0b0001);	// Chip Select  PIN
	MGPIO_VidSetPinDirection(GPIOA, PIN5, 0b1001);
	MGPIO_VidSetPinDirection(GPIOA, PIN6, INPUT_FLOATING);					// MISO PIN - Not IN USE
	MGPIO_VidSetPinDirection(GPIOA, PIN7, 0b1001);

	MGPIO_VidSetPinValue(GPIOA, PIN4 , HIGH);
	MGPIO_VidSetPinValue(GPIOA, PIN3 , HIGH);
	SET_BIT(AFIO->MAPR , 25);
	SPI1_voidInit();
	Reader_init();
	TIMER_voidInit();
	//UART1_voidInit();
	/*Enable RCC for crc*/
	//RCC_voidEnableClockPrephiral(RCC_AHB , 6);
	/*Enable RCC for GPIOB*/
	//RCC_voidEnableClockPrephiral(RCC_APB2 , 2);
	//RCC_voidEnableClockPrephiral(RCC_AHB , 4);
	//RCC_voidEnableClockPrephiral(RCC_APB2 , 14);
	//MGPIO_VidSetPinDirection( GPIOA ,PIN9 ,0b1001);
	//MGPIO_VidSetPinDirection( GPIOA ,PIN10 ,0b0100);
	 loginData_Type patient={"29809011103009",0,"12345678",0};
	// loginData_Type patient1;
	// patient.id_w[ID_WRITE_LEN]=0x00;

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




