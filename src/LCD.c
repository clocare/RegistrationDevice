/*****************************************************************************************************************/
/* Author  : Eman ashraf                                                                                         */
/* version : v01      lcd driver                                                                                 */
/* Date    :                                                                                                     */ 
/*****************************************************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h" 

#include "UART_interface.h"
#include "LCD.h"




// init function 



void LCD_set_sys_config( u8 Baudrate,  u8 sys_par1)
{
	 u8 set_sys_config_cmd[] = { 0xAA, 0xe0, 0x55, 0xaa, 0x5a, 0xa5, 0x07,
			0x00, 0x00, 0xCC, 0x33, 0xC3, 0x3C };
	MUSART1_voidTransmit(set_sys_config_cmd);
}

void LCD_hand_shake(u8 * ptr_response) {          
	u8 hand_shake_cmd[] = { 0xAA, 0x30, 0xCC, 0x33, 0xC3, 0x3C };
	UART1_voidTransmitSync(hand_shake_cmd,6);
	UART1_voidRecieveSync(ptr_response,23);
}



void LCD_STR_write(u32 VP_STR_Address,u8 *ptr_data) {

   //calculate address 	
	u8 Addr3 = (VP_STR_Address >> 24);
	u8 Addr2 = (VP_STR_Address >> 16);
	u8 Addr1 = (VP_STR_Address >> 8);
	u8 Addr0 = (u8) VP_STR_Address;
	// send packet header and address 
	u8 STR_write_cmd[] = { 0xAA, 0x42, Addr3, Addr2, Addr1, Addr0 };
	u8 tail_cmd[] = { 0xCC, 0x33, 0xC3, 0x3C };
	UART1_voidTransmitSync(STR_write_cmd,6);
	
	//string to write 
	for (u8 i = 0; i <= 127; i++) {
		UART1_voidTransmitSync(&ptr_data[i],1);
		if (ptr_data[i] == 0x00)//null
			break;
	}
	//send packet tail 
	UART1_voidTransmitSync(tail_cmd,4);
	
	
}

void LCD_STR_read(u32 VP_STR_Address,u8 *ptr_data) {
   	
	u8 Addr3 = (VP_STR_Address >> 24);
	u8 Addr2 = (VP_STR_Address >> 16);
	u8 Addr1 = (VP_STR_Address >> 8);
	u8 Addr0 = (u8) VP_STR_Address;
	u8 STR_read_cmd[] = { 0xAA, 0x43, Addr3, Addr2, Addr1, Addr0, 0xCC,
			0x33, 0xC3, 0x3C };
	UART1_voidTransmitSync(STR_read_cmd, 11);
	UART1_voidRecieveSync(ptr_data, 2); //read Communication packet header and Command executed
	u8 i;
	for (i = 2; i <= 128; i++) {
		UART1_voidRecieveSync(&ptr_data[i], 1);
		
		if (ptr_data[i] == 0x00)
			break;
	}
	i = i + 1;
	UART1_voidRecieveSync(&ptr_data[i], 4);   //read Communication packet tail // we need to optimize recive data in uart.h 
	
}

void LCD_N16_write(u32 VP_N16_Address, u16 data) {        //tested
	u8 Addr3 = (VP_N16_Address >> 24);
	u8 Addr2 = (VP_N16_Address >> 16);
	u8 Addr1 = (VP_N16_Address >> 8);
	u8 Addr0 = (u8) VP_N16_Address;
	u8 High_Byte = (data >> 8);
	u8 Low_Byte = (u8) data;
	u16 N16_write_cmd[] = { 0xAA, 0x3D, Addr3, Addr2, Addr1, Addr0,
			High_Byte, Low_Byte, 0xCC, 0x33, 0xC3, 0x3C };
	UART1_voidTransmitSync(N16_write_cmd, 12);
}

u16 LCD_N16_read(u32 VP_N16_Address) {               //tested
	u8 Response[8];
	u16 data;
	u8 Addr3 = (VP_N16_Address >> 24);
	u8 Addr2 = (VP_N16_Address >> 16);
	u8 Addr1 = (VP_N16_Address >> 8);
	u8 Addr0 = (u8) VP_N16_Address;
	u8 N16_read_cmd[] = { 0xAA, 0x3E, Addr3, Addr2, Addr1, Addr0, 0xCC,
			0x33, 0xC3, 0x3C };
	UART1_voidTransmitSync(N16_read_cmd, 10);
	UART1_voidRecieveSync(Response, 8);
	data = (Response[2] << 8) + Response[3];
	return data;
}
/*u46 LCD_N64_read(u32 VP_N46_Address) {               //should add unsigned long long in typedef file 
	u46 data;
	u8 Response[14];
	u8 Addr3 = (VP_N46_Address >> 24);
	u8 Addr2 = (VP_N46_Address >> 16);
	u8 Addr1 = (VP_N46_Address >> 8);
	u8Addr0 = (u8) VP_N46_Address;
	u8 N32_read_cmd[] = { 0xAA, 0x45, Addr3, Addr2, Addr1, Addr0, 0xCC,
			0x33, 0xC3, 0x3C };
	UART1_voidTransmitSync(N32_read_cmd, 10);
	UART1_voidRecieveSync(Response, 14);
	data = (Response[2] << 48) + (Response[3] << 40) + (Response[4] << 32)
			+ (Response[5] << 24)+(Response[6] << 16)+(Response[7] << 8)+(Response[8] )+(Response[9] );
	return data;
	
}*/
u8 LCD_GET_TouchKey_press(void) {
	u8 Response[9];
	u8 cmd;
	u8 rx_Page_ID;
	u8 rx_Key_ID;
	//while (1) {
		UART1_voidRecieveSync(Response, 10);
		//cmd = Response[2];
		//rx_Page_ID = (Response[3] << 8) + Response[4];
		rx_Key_ID = Response[5];
		
			return rx_Key_ID;

	}
	
u16	LCD_Read_Pg_Id(void)
	{
		u8 Response[8];
		u16 Page_Id=0;
		//AA 32 CC 33 C3 3C
		u8 Id_read_cmd[]={0xAA, 0x32, 0xCC,
			0x33, 0xC3, 0x3C };
	 UART1_voidTransmitSync(Id_read_cmd, 6);
	 
	 UART1_voidRecieveSync(Response, 8);
	 Page_Id=Response[2]<<8 + Response[3];
	 return Page_Id ; 
	}

//UART1_voidRecieveSync
//UART1_voidTransmitSync
