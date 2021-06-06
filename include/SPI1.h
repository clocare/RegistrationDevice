/********************************************************/ 
/* Author : Nourhan Mansour                             */
/* Date   : 8/5/2021                                    */
/* Version: 1.0.0                                       */
/* File   : SPI1.h 					                            */
/********************************************************/ 
#ifndef SPI1_H
#define SPI1_H

#include "STD_TYPES.h"
#include "SPI1_cfg.h"

void SPI1_voidInit(void);

u16 SPI1_u8SendRecieveSync(u16 data);

void SPI1_voidStop(void);

#endif
