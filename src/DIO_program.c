/*********************************************************************************/
/* Author    : Ahmed Atia                                                        */
/* Version   : V01                                                               */
/* Date      : 12 August 2020                                                    */
/*********************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "DIO_private.h"
#include "DIO_config.h"

void MGPIO_VidSetPinDirection( u8 Copy_u8Port , u8 Copy_u8Pin , u8 u8Copy_u8Mode ){

	switch(Copy_u8Port)
	{
	case GPIOA:

		if(Copy_u8Pin <= 7 )
		{//low

			GPIOA_CRL &= ~ ( ( 0b1111 )    << ( Copy_u8Pin * 4 ));// R M W
			GPIOA_CRL |= ( u8Copy_u8Mode ) << ( Copy_u8Pin * 4 );

		}
		else if(Copy_u8Pin <=15 )
		{//high

			Copy_u8Pin = Copy_u8Pin - 8;
			GPIOA_CRH &= ~ ( ( 0b1111 )    << ( Copy_u8Pin * 4 ) );
			GPIOA_CRH |= ( u8Copy_u8Mode ) << ( Copy_u8Pin * 4 );
		}

		break;

	case GPIOB:

		if(Copy_u8Pin <= 7 )
		{//low

			GPIOB_CRL &= ~ ( ( 0b1111 )    << ( Copy_u8Pin * 4 ) );// R M W
			GPIOB_CRL |= ( u8Copy_u8Mode ) << ( Copy_u8Pin * 4 )  ;

		}
		else if(Copy_u8Pin <=15 )
		{//high
			Copy_u8Pin = Copy_u8Pin - 8;
			GPIOB_CRH &= ~ ( ( 0b1111 )    << ( Copy_u8Pin * 4 ) );
			GPIOB_CRH |= ( u8Copy_u8Mode ) << ( Copy_u8Pin * 4 )  ;
		}
		break;
	case GPIOC:
		if(Copy_u8Pin <= 7 )
		{//low
			GPIOC_CRL &= ~ ( ( 0b1111 )    << ( Copy_u8Pin * 4 ) );// R M W
			GPIOC_CRL |= ( u8Copy_u8Mode ) << ( Copy_u8Pin * 4 )  ;
		}
		else if(Copy_u8Pin <=15 )
		{//high
			Copy_u8Pin = Copy_u8Pin - 8;
			GPIOC_CRH &= ~ ( ( 0b1111 )    << ( Copy_u8Pin * 4 ) );
			GPIOC_CRH |= ( u8Copy_u8Mode ) << ( Copy_u8Pin * 4 )  ;
		}
		break;
	default :break;
	}


}

void MGPIO_VidSetPinValue( u8 Copy_u8Port , u8 Copy_u8Pin , u8 u8Copy_u8Value ){

	switch(Copy_u8Port)
	{
	case GPIOA:
		if( u8Copy_u8Value == HIGH ){

			SET_BIT( GPIOA_ODR , Copy_u8Pin );

		}else if( u8Copy_u8Value == LOW ){
			CLR_BIT(  GPIOA_ODR , Copy_u8Pin );
		}
		break;
		
	case GPIOB:
		if( u8Copy_u8Value == HIGH ){

			SET_BIT( GPIOB_ODR  , Copy_u8Pin );

		}else if( u8Copy_u8Value == LOW ){
			CLR_BIT( GPIOB_ODR  , Copy_u8Pin );
		}
		break;
		
	case GPIOC:
		if( u8Copy_u8Value == HIGH ){

			SET_BIT( GPIOC_ODR , Copy_u8Pin );

		}else if( u8Copy_u8Value == LOW ){
			CLR_BIT( GPIOC_ODR , Copy_u8Pin );
		}
		break;

	}

}

u8   MGPIO_u8GetPinValue( u8 Copy_u8Port , u8 Copy_u8Pin ){

	u8 LOC_u8Result = 0 ;

	switch(Copy_u8Port)
	{
	case GPIOA:
		LOC_u8Result = GET_BIT( GPIOA_IDR , Copy_u8Pin );

		break;
	case GPIOB:

		LOC_u8Result = GET_BIT( GPIOB_IDR , Copy_u8Pin );

		break;
	case GPIOC:

		LOC_u8Result = GET_BIT( GPIOC_IDR , Copy_u8Pin );

		break;
	}
	return LOC_u8Result;
}

void SEVENSEGEMENT_voidDisplay (u8 copy_u8Port, u8 copy_u8Common,u8 copy_u8NUM )
	{
		switch(copy_u8Port)
		{
		case GPIOA :
	         if(copy_u8Common==COMMON_CASODE)
			 {
				 GPIOA_ODR=0 ;/*intialize pins*/
			 if(copy_u8NUM==0){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN6 , LOW);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
				 				 }

			 else if(copy_u8NUM==1){
					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , LOW);
					 		MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
					 		MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
					 		MGPIO_VidSetPinValue(GPIOA ,PIN3 , LOW);
					 		MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
					 		MGPIO_VidSetPinValue(GPIOA ,PIN5 , LOW);
					 		MGPIO_VidSetPinValue(GPIOA ,PIN6 , LOW);
					 		MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
				 }
				 else if(copy_u8NUM==2){
					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN2 , LOW);
					MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN5 , LOW);
					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
				 }else if(copy_u8NUM==3){
					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
					MGPIO_VidSetPinValue(GPIOA ,PIN5 , LOW);
					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
					MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
				 }else if(copy_u8NUM==4){
 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
 				 }else if(copy_u8NUM==5){
 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
 				 }else if(copy_u8NUM==6){
 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
 				 }else if(copy_u8NUM==7){
 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , LOW);
 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
 				 }
 				else if(copy_u8NUM==8){
 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
						MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
 								 				 }else if(copy_u8NUM==9){
 								 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
 								 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
 								 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
 								 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , LOW);
 								 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
 								 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
 								 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
 								 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
 								 				 }

			 }


			 	else if(copy_u8Common==COMMON_ANODE ){
				 GPIOA_ODR=0 ;
				 if(copy_u8NUM==1){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , LOW);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN1 , LOW);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
				 					 		MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
				 				 }
				 				 else if(copy_u8NUM==2){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
				 				 }else if(copy_u8NUM==3){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
				 				 }else if(copy_u8NUM==4){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
				 				 }else if(copy_u8NUM==5){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
				 				 }else if(copy_u8NUM==6){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
				 				 }else if(copy_u8NUM==7){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , LOW);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , LOW);
				 				 }
				 				else if(copy_u8NUM==8){
				 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
			 						MGPIO_VidSetPinValue(GPIOA ,PIN3 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
				 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
				 								 				 }else if(copy_u8NUM==9){
				 								 					 MGPIO_VidSetPinValue(GPIOA ,PIN0 , HIGH);
				 								 					MGPIO_VidSetPinValue(GPIOA ,PIN1 , HIGH);
				 								 					MGPIO_VidSetPinValue(GPIOA ,PIN2 , HIGH);
				 								 					MGPIO_VidSetPinValue(GPIOA ,PIN3 , LOW);
				 								 					MGPIO_VidSetPinValue(GPIOA ,PIN4 , HIGH);
				 								 					MGPIO_VidSetPinValue(GPIOA ,PIN5 , HIGH);
				 								 					MGPIO_VidSetPinValue(GPIOA ,PIN6 , HIGH);
				 								 					MGPIO_VidSetPinValue(GPIOA ,PIN7 , HIGH);
				 								 				 }



			 }
			 break ;


		case GPIOB:
	         if(copy_u8Common==COMMON_CASODE)
			 {
				 GPIOB_ODR=0 ;
				 GPIOB_ODR= copy_u8NUM ;


			 }	else if(copy_u8Common==COMMON_ANODE ){
				 GPIOB_ODR=0 ;
				 GPIOB_ODR=~copy_u8NUM ;
			 }
			 break ;


		case GPIOC:
	         if(copy_u8Common==COMMON_CASODE)
			 {
				 GPIOC_ODR=0 ;
				 GPIOC_ODR= copy_u8NUM ;

			 }
	         else if(copy_u8Common==COMMON_ANODE ){
				 GPIOC_ODR=0 ;
				 GPIOC_ODR=~copy_u8NUM ;
			 }
			 break ;

		}

	}

