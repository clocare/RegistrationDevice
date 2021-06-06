#ifndef FPEC_INTERFACE
#define FPEC_INTERFACE

void FPEC_voidEraseAppArea(void);
	
void FPEC_voidFlashPageErase(u8 Copy_u8PageNumber);

void FPEC_voidFlashWrite(u8 * Copy_u8Data , u8 cpyPage , u8 Copy_u8DataArrayLength , u16 offset);

u16 FPEC_u16ReadHalfWord(u8 cpyPage , u8 offset);
u32  FPEC_u32ReadWord(u8 copyPage , u8 offset1);
u32 u32reverse (u32 val);
u16 u16reverse (u32 val);


#endif

