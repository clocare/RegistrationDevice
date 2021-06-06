#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "FPEC_interface.h"
#include "FPEC_private.h"
#include "FPEC_config.h"


u16 FPEC_u16ReadHalfWord(u8 cpyPage , u8 offset)

{

	while (GET_BIT(FPEC->SR,0) == 1);

			/* Check if FPEC is locked or not */
			if ( GET_BIT(FPEC->CR,7) == 1)
			{
				FPEC -> KEYR = 0x45670123;
				FPEC -> KEYR = 0xCDEF89AB;
			}
	volatile u32 local_address = (u32)(cpyPage * 1024) + 0x08000000 + offset;
	volatile u16 val = *((volatile u16 *) local_address);
	return val;
}

u32  FPEC_u32ReadWord(u8 copyPage , u8 offset1)

{

	//volatile u32 rev=0 ;

	//volatile u32 word=0 ;
	volatile u16 val2=0;
	volatile u32 val=0;
	volatile u16 val1=0;

	val1=FPEC_u16ReadHalfWord( copyPage ,  offset1);
	//val=u16reverse(val);
   // word = val ;
	//word=word<<16;
	val=val1 ;
	val=val<<16 ;
	val2 = FPEC_u16ReadHalfWord( copyPage ,  offset1-2);

	//val2=u16reverse(val2);
	val|=val2 ;

	// word |= (u32)val ;

		return val ;
}

void FPEC_voidEraseAppArea(void)
{
	u8 i;
	
	for (i=4;i<64;i++)
	{
		FPEC_voidFlashPageErase(i);
	}
}

void FPEC_voidFlashPageErase(u8 Copy_u8PageNumber)
{
	/* Wait Busy Flag */
	while (GET_BIT(FPEC->SR,0) == 1);

	/* Check if FPEC is locked or not */
	if ( GET_BIT(FPEC->CR,7) == 1)
	{
		FPEC -> KEYR = 0x45670123;
		FPEC -> KEYR = 0xCDEF89AB;
	}
	
	/* Page Erase Operation */
	SET_BIT(FPEC->CR,1);

	/* Write Page address */
	FPEC->AR = (u32)(Copy_u8PageNumber * 1024) + 0x08000000 ;

	/* Start operation */
	SET_BIT(FPEC->CR,6);

	/* Wait Busy Flag */
	while (GET_BIT(FPEC->SR,0) == 1);

	/* EOP */
	SET_BIT(FPEC->SR,5);
	CLR_BIT(FPEC->CR,1);
}

void FPEC_voidFlashWrite(u8 * Copy_u8Data , u8 cpyPage , u8 Copy_u8DataArrayLength , u16 offset)
{
	u8 i ;
	u16 local_u16Data;
	volatile u32 local_u32address =(u32) (cpyPage * 1024) + 0x08000000 + offset;

	while (GET_BIT(FPEC->SR,0) == 1);
	if ( GET_BIT(FPEC->CR,7) == 1)
		{
			FPEC -> KEYR = 0x45670123;
			FPEC -> KEYR = 0xCDEF89AB;
		}
	CLR_BIT(FPEC->CR,PER);
	
	// Write Flash Programming
	SET_BIT(FPEC->CR,PG);
	
	for (i=0 ; i<Copy_u8DataArrayLength-1 ; i+=2)
	{
		// Half word operation
		local_u16Data = ( (u16)Copy_u8Data[i] ) + ( (u16)Copy_u8Data[i+1]<<8 );
		*((volatile u16*)local_u32address) = local_u16Data;
		local_u32address +=2;
		// wait busy flag and End of operation
		while (GET_BIT(FPEC->SR,BSY ) == 1);
		// reset EOP BIT by writing 1
		SET_BIT(FPEC->SR, EOP);
	}
}

u32 u32reverse (u32 val)
	{
		u32 rev =0 ;
		while (val != 0) {
					  u32  remainder = val % 16;
					    rev = rev * 16 + remainder;
					        val /= 16;
					    }
		return rev;
	}
u16 u16reverse (u32 val)
	{
		u32 rev =0 ;
		while (val != 0) {
					  u32  remainder = val % 16;
					    rev = rev * 16 + remainder;
					        val /= 16;
					    }
		return rev;
	}

