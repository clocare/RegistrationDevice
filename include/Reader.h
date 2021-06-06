/*******************************************************/ 
/* Author : Nourhan Mansour                            */
/* Date   : 21/5/2021                                  */
/* Version: 1.0                                        */
/* File   : Reader.h				                           */
/*******************************************************/ 

#ifndef READER_H
#define READER_H

/************************************************************************/
/*                         Global constants                             */
/************************************************************************/
#define ID_WRITE_LEN		16U			// national id len 
#define ID_READ_LEN			18U
#define ID_BLOCK				04U			// Block in tag memory

#define PASS_WRITE_LEN		16U			// pass len 
#define PASS_READ_LEN			18U
#define PASS_BLOCK				05U
/************************************************************************/
/*                          Type definitions                            */
/************************************************************************/
typedef struct loginData_Type
{
	u8 id_w[ID_WRITE_LEN];
	u8 id_r[ID_READ_LEN];
	
	u8 Pass_w[PASS_WRITE_LEN];
	u8 Pass_r[PASS_READ_LEN];
}loginData_Type;	

/************************************************************************/
/*                        Functions APIs	                              */
/************************************************************************/

void Reader_init(void);

boolean Reader_isNewCardPresent(void);

boolean Reader_SetLoginData(loginData_Type loginData );

loginData_Type Reader_GetLogin (void);


#endif
