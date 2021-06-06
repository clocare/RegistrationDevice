#/********************************************************/ 
/* Author : Nourhan Mansour	                             */
/* Date   : 8/5/2021	                                   */
/* Version: 1.0.0          	                             */
/* File   : SPI1_config.h    	                           */
/*********************************************************/ 
#ifndef SPI1_CONFIG_H
#define SPI1_CONFIG_H
     

/* Master Slave Selection */
#define SPI_MASTER_MODE         1U
#define SPI_SLAVE_MODE          0U
#define SPI1_MODE            		SPI_MASTER_MODE

/* Clk Polarity & phase  for either master or slave*/
/*
    Logic 0 when Idle = 0
    Logic 1 when Idle = 1
*/
#define SPI_CLK_POL_HIGH				1U
#define SPI_CLK_POL_LOW					0U
#define SPI1_CLK_POL         		SPI_CLK_POL_LOW

/*
    First data Transition in first clock  = 0
    First data Transition in second clock = 1 
*/
#define SPI_CLK_PHA_HIGH				 1U
#define SPI_CLK_PHA_LOW					 0U
#define SPI1_CLK_PHASE       		 SPI_CLK_PHA_LOW

/* LSB FIRST = 1 
   MSB FIRST = 0
*/ 
#define SPI_LSB_FIRST           1U
#define SPI_LSB_LAST            0U

#define SPI1_TransferStart			SPI_LSB_LAST

/* 8  Bit Data Frame  = 0 
   16 Bit Data Frame  = 1
*/
#define SPI_8BIT_DFF						0U
#define SPI_16BIT_DFF						1U

#define SPI1_DFF_MODE           SPI_8BIT_DFF 


#if SPI1_MODE == SPI_MASTER_MODE 
        
    #define SPIBAUD_RATE_CLK_DIV2           0
    #define SPIBAUD_RATE_CLK_DIV4           1
    #define SPIBAUD_RATE_CLK_DIV8           2
    #define SPIBAUD_RATE_CLK_DIV16          3
    #define SPIBAUD_RATE_CLK_DIV32          4
    #define SPIBAUD_RATE_CLK_DIV64          5
    #define SPIBAUD_RATE_CLK_DIV128         6
    #define SPIBAUD_RATE_CLK_DIV256         7

    #define SPI1_BAUD_RATE_SET               SPIBAUD_RATE_CLK_DIV8
#endif

/* Only SW implementation of Chip Select pin is allowed here */

#endif // End Of File
