/********************************************************/
/* Author : Nourhan Mansour                             */
/* Date   : 8/5/2021                                    */
/* Version: 1.0.0                                       */
/* File   : SPI1.c 				                              */
/********************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "hw_reg.h"

#include "SPI1.h"
#include "SPI1_cfg.h"

/************************************************************************/
/*                          Global Constants                            */
/************************************************************************/

#define PORTA_CHANNEL_OFFEST (0U)
#define PORTB_CHANNEL_OFFEST (16U)
#define PORTC_CHANNEL_OFFEST (32U)
#define PORTx_CHANNEL_OFFSET (48U)

/************************************************************************/
/*                          Type Definition                             */
/************************************************************************/

// typedef uint8 CS_PIN_Type;
/************************************************************************/
/*                          Local functions                             */
/************************************************************************/
// static void Dio_WriteChannel(CS_PIN_Type ChannelId, Std_ReturnType Level);

/************************************************************************/
/*                         Global variables                             */
/************************************************************************/


/************************************************************************/
/*                         APIS definitions                             */
/************************************************************************/
void SPI1_voidInit(void)
{
	    // CLEAR CFG
    SPI1_CR1 = 0;
    SPI1_CR2 = 0;

    #if SPI1_MODE == SPI_MASTER_MODE
        // Select the BR[2:0] bits to define the serial clock baud rate
        SPI1_CR1 |= ( SPI1_BAUD_RATE_SET << SPI_CR1_BR0 );
        // Set MSTR
        SET_BIT (SPI1_CR1  , SPI_CR1_MSTR);
    #elif  SPI1_MODE == SPI_SLAVE_MODE
       // Clear the MSTR bit
        SET_BIT (SPI1_CR1  , SPI_CR1_MSTR);
    #endif
    // Enable SW slave managment
    SET_BIT(SPI1_CR1 , SPI_CR1_SSI);
    SET_BIT(SPI1_CR1 , SPI_CR1_SSM);

    // define 8- or 16-bit data frame format
    #if SPI1_DFF_MODE == SPI_8BIT_DFF
        CLR_BIT (SPI1_CR1 , SPI_CR1_DFF);    // 8-Bit data Selected
    #else
        SET_BIT (SPI1_CR1 , SPI_CR1_DFF);    // 16-Bit data Selected
    #endif

    // Select the CPOL and CPHA
    #if SPI1_CLK_POL == SPI_CLK_POL_LOW
        CLR_BIT (SPI1_CR1 , SPI_CR1_CPOL);       //idle = 0
    #else
        SET_BIT (SPI1_CR1 , SPI_CR1_CPOL);       //idle = 1
    #endif

    #if SPI1_CLK_PHASE == SPI_CLK_PHA_LOW
        CLR_BIT (SPI1_CR1 , SPI_CR1_CPHA);       // Data on First Clock
    #else
        SET_BIT (SPI1_CR1 , SPI_CR1_CPHA);       // Data on Second Clock
    #endif

    // frame format (MSB-first or LSB-first)
    #if SPI1_TransferStart == SPI_LSB_FIRST
        SET_BIT (SPI1_CR1 , SPI_CR1_LSBF);       // LSB sent first
    #else
        CLR_BIT (SPI1_CR1 , SPI_CR1_LSBF);       // MSB sent first
    #endif
    // ENABLE SPI
    SET_BIT (SPI1_CR1  , SPI_CR1_SPE);
	return ;
}

u16 SPI1_u8SendRecieveSync(u16 data)
{
	  // Clear ss pin
//    Dio_WriteChannel( SPI1_CS_PIN ,  STD_LOW);
   // Wait for Busy Flag
    while ( GET_BIT(SPI1_SR , SPI_SR_TXE ) == 0 );
    // Send Data
    SPI1_DR = (u32)data;
    // Wait for Busy Flag
    while (GET_BIT(SPI1_SR , SPI_SR_RXNE ) == 0 );
    // set SS pin
//    Dio_WriteChannel(SPI1_CS_PIN ,  STD_HIGH);
    // Return Recieved data
    return (SPI1_DR & 0xffff);
}

void SPI1_voidStop(void)
{
	  // Wait RXNE = 1
    while (GET_BIT(SPI1_SR , SPI_SR_RXNE ) != 1);
    // Wait TXE  = 1
    while (GET_BIT(SPI1_SR , SPI_SR_TXE ) != 1);
    // Wait BSY  = 0
    while (GET_BIT(SPI1_SR , SPI_SR_BSY ) != 0);
    // Clear SPE
    CLR_BIT(SPI1_CR1 , SPI_CR1_SPE );
	return ;
}

