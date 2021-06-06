/*****************************************************************************************************************/
/* Author  : Eman ashraf                                                                                         */
/* version : v01                                                                                                 */
/* Date    :                                                                                                     */ 
/*****************************************************************************************************************/
#ifndef  LCD_H
#define  LCD_H




/****function declarations ******/  
/*
  Descrption: Baud Rate and system parameter configuratio
   paramter:  
*/  
void LCD_set_sys_config(u8 Baudrate, u8 sys_par1);
/*
   
  Description : Read a Hand Shake
  Input :
   ptr_response: pointer to the response code

 */
void LCD_hand_shake(u8 * ptr_response); 
/*
   Descrition:write string to VP_STR
   input:  address range from 0x00000 to 0x01FFFF
   
*/
void LCD_STR_write(u32 VP_STR_Address,u8 *ptr_data); 

 /* Description : Read string from VP_STR
  Input :
  			VP_STR_Address = 0x00000 ~ 0x01FFFF, (each VP_STR = 128 bytes)
                            (address value must be divisible by 128)
  Output :
          ptr_data : pointer to string to read
 */

void LCD_STR_read(u32 VP_STR_Address,u8 *ptr_data);
/*
  
    Description : Write 16bit number to VP_N16
    Input :
  			VP_N16_Address = 0x080000 ~ 0x08FFFF,(each VP_N16 = 2 byte)
                            (address value must be divisible by 2)
          data : The 16 bit value to write
 */
void LCD_N16_write(u32 VP_N16_Address, u16 data);

/*
 * 
   Description : read 16bit number from VP_N16
   Input :
    VP_N16_Address = 0x080000 ~ 0x08FFFF,(each VP_N16 = 2 byte)
                           (address value must be divisible by 2)
    output:  data  of 16 bit value .
		   
 */
 u16 LCD_N16_read(u32 VP_N16_Address);
 /*
    Description :  read 16bit number from VP_N64
	Input :
    VP_N16_Address = 0x020000 ~ 0x02FFFF,(each VP_N64 = 8 byte)
                           (address value must be divisible by 8)
    output:  data  of 64 bit value .
 
 */
 
 //u46 LCD_N64_read(u32 VP_N46_Address);
 /*
    Description : get response ID
   inpt: no
  output: press key Id.   
 
 */
 
 u8 LCD_GET_TouchKey_press(void);
 /*
    Description : get page ID 
	input: no
	output : page Id of u16 
 
 */
 
 u16 LCD_Read_Pg_Id(void);

#endif
