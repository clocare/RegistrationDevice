/******************************************************/
/* Author    : Nourhan MAnsour                        */
/* Date      : 28/1/2021                              */
/* Version   : 1.0                                    */
/* File      : HW_reg.h                               */
/******************************************************/

#ifndef HW_REG_H
#define HW_REG_H

/*********************** PORT A **********************/


/*********************** AF **********************/

#define AFIO_BASE_ADDRESS       0x40010000
#define AFIO                    ((t_AFIO *) AFIO_BASE_ADDRESS)
typedef struct 
{
   volatile u32 EVCR;
   volatile u32 MAPR;
   volatile u32 EXTICR[4];

}t_AFIO;

/************************* SPI *************************/

#define SPI1_BASE           (0x40013000)
#define SPI2_BASE           (0x40003800)
#define SPI3_BASE           (0x40013C00)

/********** Registers offsets ****************/
#define SPI_CR1_OFFSET          0x00
#define SPI_CR2_OFFSET          0x04
#define SPI_SR_OFFSET           0x08
#define SPI_DR_OFFSET           0x0C
#define SPI_CRCPR_OFFSET        0x10
#define SPI_RXCRCR_OFFSET       0x14
#define SPI_TXCRCR_OFFSET       0x18
#define SPI_I2SCFGR_OFFSET      0x1C
#define SPI_I2SPR_OFFSET        0x20

/********************** SPI1 Registers ***************/
#define SPI1_CR1               *((volatile u32 *) (SPI1_BASE + SPI_CR1_OFFSET))
#define SPI1_CR2               *((volatile u32 *) (SPI1_BASE + SPI_CR2_OFFSET))
#define SPI1_SR                *((volatile u32 *) (SPI1_BASE + SPI_SR_OFFSET))
#define SPI1_DR                *((volatile u32 *) (SPI1_BASE + SPI_DR_OFFSET))
#define SPI1_CRCPR             *((volatile u32 *) (SPI1_BASE + SPI_CRCPR_OFFSET))
#define SPI1_RXCRCR            *((volatile u32 *) (SPI1_BASE + SPI_RXCRCR_OFFSET))
#define SPI1_TXCRCR            *((volatile u32 *) (SPI1_BASE + SPI_TXCRCR_OFFSET))
#define SPI1_I2SCFGR           *((volatile u32 *) (SPI1_BASE + SPI_I2SCFGR_OFFSET))
#define SPI1_I2SPR             *((volatile u32 *) (SPI1_BASE + SPI_I2SPR_OFFSET))


/******** Registers Bits *************/

#define SPI_CR1_CPHA        0
#define SPI_CR1_CPOL        1
#define SPI_CR1_MSTR        2
#define SPI_CR1_BR0         3
#define SPI_CR1_BR1         4
#define SPI_CR1_BR2         5
#define SPI_CR1_SPE         6
#define SPI_CR1_LSBF        7
#define SPI_CR1_SSI         8
#define SPI_CR1_SSM         9
#define SPI_CR1_RXONLY      10
#define SPI_CR1_DFF         11
#define SPI_CR1_CRCNXT      12
#define SPI_CR1_CRCEN       13
#define SPI_CR1_BIDIOE      14
#define SPI_CR1_BIDIMODE    15

/***************************************/

#define SPI_CR2_RXDMAEN     0
#define SPI_CR2_TXDMAEN     1
#define SPI_CR2_SSOE        2
#define SPI_CR2_ERRIE       5
#define SPI_CR2_RXNEIE      6
#define SPI_CR2_TXEIE       7

/***************************************/

#define SPI_SR_RXNE         0
#define SPI_SR_TXE          1
#define SPI_SR_CHSIDE       2
#define SPI_SR_UDR          3
#define SPI_SR_CRCERR       4
#define SPI_SR_MODF         5
#define SPI_SR_OVR          6
#define SPI_SR_BSY          7

#endif
