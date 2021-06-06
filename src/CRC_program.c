/*****************************************************************************************************************/
/* Author  : Eman ashraf                                                                                         */
/* version : v01                                                                                                 */
/* Date    : 11\11\2020                                                                                        */ 
/*****************************************************************************************************************/
 #include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "CRC_interface.h"
#include "CRC_private.h"
#include "CRC_config.h"

/**
  * @brief  Computes the 32-bit CRC of a array of data word(32-bit).
  * @param  Copy_u16Data: pointer to the array containing the data to be computed
  * @param  Copy_u16Datalength: length of the array to be computed					
  * @retval 32-bit CRC
  */
u32 CRC_u32Accumalate(u32 *Copy_u32Data , u32 Copy_u32Datalength)
{
	u32 i ;
	// Enter Data to the CRC calculator
  for(i = 0; i < Copy_u32Datalength; i++)
  {
   CRC->CRC_DR = Copy_u32Data[i];
  }
  
  return CRC->CRC_DR ;
}
 /**
  * @brief  Computes the 32-bit CRC data word(32-bit).
  * @param  Copy_u16Data: data to be computed		
  * @retval 32-bit CRC
  */

u32 CRC_u32Claculate(u32 Copy_u32Data)
{
	 CRC->CRC_DR = Copy_u32Data ;
      return CRC->CRC_DR ;	 
}

void CRC_voidReset()
{
	CRC->CRC_CR=CRC_RESET ;
}

void CRC_u32Claculate2(u32 Copy_u32Data)
{
	 CRC->CRC_DR = Copy_u32Data ;

}
u32 CRC_u32GetCrc()
{
	return CRC->CRC_DR ;

}
u8 BL_voidValidateCRCFromFlash(u16 cpyDataLength , u32 cpyCRCValue)
{
	u32 localu32Data ;
   u16 i;
	u32 localcrc ;
for(i=0;i<cpyDataLength;i+=4 )
{
	localu32Data=FPEC_u32ReadWord(APP_PAGE,i);
	CRC_u32Claculate2(localu32Data);

}

localcrc= CRC_u32GetCrc();
if (cpyCRCValue==localcrc )
{
	return IMAGE_VERIFIED ;
}
else
{
	return IMAGE_NOT_VERIFIED ;
}

}

