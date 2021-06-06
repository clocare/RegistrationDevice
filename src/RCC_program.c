/*************************************************/
/* Author : Eslam Aboglala                       */
/* Date   : 10 Aug 2020                          */
/* Version: V01                                  */
/*************************************************/


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "RCC_private.h"
#include "RCC_config.h"

void RCC_voidInitSysClock(void)
{
	#if RCC_CLOCK_TYPE   == RCC_HSE_CRYSTAL
		RCC_CR  =0x00010000;/*Enable HSE With No bypass*/
		RCC_CFGR=0x00000001;/*Enable HSE Input Clock*/

	#elif RCC_CLOCK_TYPE == RCC_HSE_RC
		RCC_CR  =0x00050000;/*Enable HSE With bypass*/
		RCC_CFGR=0x00000001;/*Enable HSE Input Clock*/
	
	#elif RCC_CLOCK_TYPE == RCC_HSI
		RCC_CR  =0x00000081; /*Enable HSI + Trimming =0*/
		RCC_CFGR=0x00000000; /*Enable HSI Input Clock*/
	
	#elif RCC_CLOCK_TYPE == RCC_PLL
		#if   RCC_CLOCK_TYPE == RCC_PLL_IN_HSI_DIV_2
			#if   RCC_PLL_MUL_VAL == 2
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00000002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *2  */

			#elif RCC_PLL_MUL_VAL == 3
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00040002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *3  */

			#elif RCC_PLL_MUL_VAL == 4
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00080002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *4  */

			#elif RCC_PLL_MUL_VAL == 5
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x000C0002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *5  */

			#elif RCC_PLL_MUL_VAL == 6
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00100002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *6  */

			#elif RCC_PLL_MUL_VAL == 7
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00140002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *7  */

			#elif RCC_PLL_MUL_VAL == 8
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00180002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *8  */

			#elif RCC_PLL_MUL_VAL == 9
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x001C0002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *9  */

			#elif RCC_PLL_MUL_VAL == 10
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00200002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *10 */

			#elif RCC_PLL_MUL_VAL == 11
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00240002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *11 */

			#elif RCC_PLL_MUL_VAL == 12
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00280002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *12 */

			#elif RCC_PLL_MUL_VAL == 13
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x002C0002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *13 */

			#elif RCC_PLL_MUL_VAL == 14
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00300002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *14 */

			#elif RCC_PLL_MUL_VAL == 15
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00340002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *15 */

			#elif RCC_PLL_MUL_VAL == 16
				RCC_CR  =0x01000081;	/*Enable HSI & PLL + Trimming =0                   */
				RCC_CFGR=0x00380002; 	/*Enable PLL Input Clock Using HSI/2 + MUL_VAL *16 */

			#else
				#error("You choosed Wrong PLL multiblication value")
			#endif
		
		#elif RCC_CLOCK_TYPE == RCC_PLL_IN_HSE_DIV_2
			#if   RCC_PLL_IN_HSE_TYPE == RCC_PLL_IN_HSE_CRYSTAL
				#if   RCC_PLL_MUL_VAL == 2
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00030002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *2 */

				#elif RCC_PLL_MUL_VAL == 3
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00070002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *3 */

				#elif RCC_PLL_MUL_VAL == 4
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x000B0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *4 */

				#elif RCC_PLL_MUL_VAL == 5
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x000F0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *5 */

				#elif RCC_PLL_MUL_VAL == 6
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00130002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *6 */

				#elif RCC_PLL_MUL_VAL == 7
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00170002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *7 */

				#elif RCC_PLL_MUL_VAL == 8
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x001B0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *8 */

				#elif RCC_PLL_MUL_VAL == 9
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x001F0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *9 */

				#elif RCC_PLL_MUL_VAL == 10
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00230002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *10*/

				#elif RCC_PLL_MUL_VAL == 11
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00270002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *11*/

				#elif RCC_PLL_MUL_VAL == 12
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x002B0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *12*/

				#elif RCC_PLL_MUL_VAL == 13
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x002F0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *13*/

				#elif RCC_PLL_MUL_VAL == 14
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00330002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *14*/

				#elif RCC_PLL_MUL_VAL == 15
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00370002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *15*/

				#elif RCC_PLL_MUL_VAL == 16
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x003B0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *16*/

				#else
					#error("You choosed Wrong PLL multiblication value")
				#endif
			#elif RCC_PLL_IN_HSE_TYPE == RCC_PLL_IN_HSE_CRYSTAL
				#if   RCC_PLL_MUL_VAL == 2
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00030002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *2  */

				#elif RCC_PLL_MUL_VAL == 3
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00070002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *3  */

				#elif RCC_PLL_MUL_VAL == 4
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x000B0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *4  */

				#elif RCC_PLL_MUL_VAL == 5
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x000F0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *5  */

				#elif RCC_PLL_MUL_VAL == 6
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00130002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *6  */

				#elif RCC_PLL_MUL_VAL == 7
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00170002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *7  */

				#elif RCC_PLL_MUL_VAL == 8
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x001B0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *8  */

				#elif RCC_PLL_MUL_VAL == 9
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x001F0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *9  */

				#elif RCC_PLL_MUL_VAL == 10
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00230002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *10 */

				#elif RCC_PLL_MUL_VAL == 11
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00270002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *11 */

				#elif RCC_PLL_MUL_VAL == 12
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x002B0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *12 */

				#elif RCC_PLL_MUL_VAL == 13
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x002F0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *13 */

				#elif RCC_PLL_MUL_VAL == 14
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00330002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *14 */

				#elif RCC_PLL_MUL_VAL == 15
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00370002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *15 */

				#elif RCC_PLL_MUL_VAL == 16
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x003B0002; 	/*Enable PLL Input Clock Using HSE/2 + MUL_VAL *16 */

				#else
					#error("You choosed Wrong PLL multiblication value")
				#endif
			#endif
		#elif RCC_CLOCK_TYPE == RCC_PLL_IN_HSE
			#if   RCC_PLL_IN_HSE_TYPE == RCC_PLL_IN_HSE_CRYSTAL
				#if   RCC_PLL_MUL_VAL == 2
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00010002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *2  */

				#elif RCC_PLL_MUL_VAL == 3
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00050002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *3  */

				#elif RCC_PLL_MUL_VAL == 4
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00090002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *4  */

				#elif RCC_PLL_MUL_VAL == 5
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x000D0002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *5  */

				#elif RCC_PLL_MUL_VAL == 6
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00110002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *6  */

				#elif RCC_PLL_MUL_VAL == 7
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00150002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *7  */

				#elif RCC_PLL_MUL_VAL == 8
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00190002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *8  */

				#elif RCC_PLL_MUL_VAL == 9
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x001D0002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *9  */

				#elif RCC_PLL_MUL_VAL == 10
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00210002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *10 */

				#elif RCC_PLL_MUL_VAL == 11
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00250002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *11 */

				#elif RCC_PLL_MUL_VAL == 12
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00290002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *12 */

				#elif RCC_PLL_MUL_VAL == 13
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x002D0002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *13 */

				#elif RCC_PLL_MUL_VAL == 14
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00310002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *14 */

				#elif RCC_PLL_MUL_VAL == 15
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00350002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *15 */

				#elif RCC_PLL_MUL_VAL == 16
					RCC_CR  =0x01010000;    /*Enable HSE With No bypass & Enable PLL          */
					RCC_CFGR=0x00390002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *16 */

				#else
					#error("You choosed Wrong PLL multiblication value")
				#endif
			#elif RCC_PLL_IN_HSE_TYPE == RCC_PLL_IN_HSE_RC
				#if   RCC_PLL_MUL_VAL == 2
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00010002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *2   */

				#elif RCC_PLL_MUL_VAL == 3
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00050002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *3   */

				#elif RCC_PLL_MUL_VAL == 4
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00090002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *4   */

				#elif RCC_PLL_MUL_VAL == 5
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x000D0002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *5   */

				#elif RCC_PLL_MUL_VAL == 6
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00110002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *6   */

				#elif RCC_PLL_MUL_VAL == 7
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00150002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *7   */

				#elif RCC_PLL_MUL_VAL == 8
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00190002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *8   */

				#elif RCC_PLL_MUL_VAL == 9
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x001D0002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *9   */

				#elif RCC_PLL_MUL_VAL == 10
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00210002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *10  */

				#elif RCC_PLL_MUL_VAL == 11
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00250002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *11  */

				#elif RCC_PLL_MUL_VAL == 12
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00290002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *12  */

				#elif RCC_PLL_MUL_VAL == 13
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x002D0002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *13  */

				#elif RCC_PLL_MUL_VAL == 14
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00310002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *14  */

				#elif RCC_PLL_MUL_VAL == 15
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00350002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *15  */

				#elif RCC_PLL_MUL_VAL == 16
					RCC_CR  =0x01050000;    /*Enable HSE With bypass & Enable PLL              */
					RCC_CFGR=0x00390002; 	/*Enable PLL Input Clock Using HSE  + MUL_VAL *16  */

				#else
					#error("You choosed Wrong PLL multiblication value")
				#endif
			#endif
		#endif
	#else
		#error("You choosed Wrong Clock type")
	#endif
}



void RCC_voidEnableClockPrephiral (u8 Copy_u8BusId,u8 Copy_u8PrephiralId)
{
	if (Copy_u8PrephiralId <= 31)
	{
		switch(Copy_u8BusId)
		{
			case RCC_AHB : SET_BIT(RCC_AHBENR  , Copy_u8PrephiralId); break;
			case RCC_APB1: SET_BIT(RCC_APB1ENR , Copy_u8PrephiralId); break;
			case RCC_APB2: SET_BIT(RCC_APB2ENR , Copy_u8PrephiralId); break;
		//	default      : /* Return Error */                         break;
		}
	}
	else
	{
		/* Return Error*/
	}
}

void RCC_voidDisableClockPrephiral(u8 Copy_u8BusId,u8 Copy_u8PrephiralId)
{
	if (Copy_u8PrephiralId <= 31)
	{
		switch(Copy_u8BusId)
		{
			case RCC_AHB : CLR_BIT(RCC_AHBENR  , Copy_u8PrephiralId); break;
			case RCC_APB1: CLR_BIT(RCC_APB1ENR , Copy_u8PrephiralId); break;
			case RCC_APB2: CLR_BIT(RCC_APB2ENR , Copy_u8PrephiralId); break;
		//	default      : /* Return Error */                         break;
		}
	}
	else
	{
		/* Return Error*/
	}

}


