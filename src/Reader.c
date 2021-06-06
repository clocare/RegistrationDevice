
// 1- include Libraries 
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// 2- include interface file of needed lower layers
#include "mfrc522.h"

// 3- include interface files
#include "Reader.h"
//#include "Observer.h"

MIFARE_Key key;


/************************************************************************/
/*														Local Functions														*/
/************************************************************************/
static boolean Reader_AuthCard(u8 block);
static boolean Reader_WriteLoginData ( loginData_Type login);
static boolean Reader_ReadID (loginData_Type * login);
/************************************************************************/
/*                        APIs Definitions                              */
/************************************************************************/

/******************************************
 * @name : Reader_init
 * @param:  
 * Non Reentrant
 * Sync
 * Service to init Reader.
 ******************************************/
void Reader_init(void)
{
	// initialize RFID module
	PCD_Init();
	// initalize keys 
	u8 i;
	for ( i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

	return;
}

/******************************************
 * @name : Reader_isNewCardPresent
 * @param:  
 * Non Reentrant
 * Sync
 * Service to query RFID reader.
 ******************************************/
boolean Reader_isNewCardPresent(void)
{
	boolean ret = FALSE; 
	if (PICC_IsNewCardPresent())
	{	
		PICC_ReadCardSerial();
		ret = TRUE; 
	}
	return ret; 
}

/******************************************
 * @name : Reader_SetId
 * @param: Id : new ID to set on card
 * Non Reentrant
 * Sync
 * Service to set new ID in card.
 ******************************************/
boolean Reader_SetLoginData(loginData_Type login)
{
	boolean ret = FALSE; 
	ret = Reader_WriteLoginData(login);
	
	return ret;
}

 /******************************************
 * @name : Reader_GetId
 * @param:  
 * @return: Id read from card if success or zero otherwise. 
 * Non Reentrant
 * Sync
 * Service to read card ID. 
 ******************************************/
loginData_Type Reader_GetLogin (void)
{
	loginData_Type ID ;
	volatile loginData_Type ret_ID = {0};
	u8 i = 0 ;
	if (Reader_AuthCard(ID_BLOCK) == TRUE)
	{
		if (Reader_ReadID(&ID) == TRUE)
		{
			for (i=0 ; i < ID_READ_LEN ; i++)
				ret_ID.id_r[i] = ID.id_r[i]; 
		}
	}
	__asm("NOP");
	return ret_ID; 
}

	
/************************************************************************/
/*									Local Functions	definitions													*/
/************************************************************************/
static boolean Reader_AuthCard(u8 block)
{
	volatile boolean ret = FALSE; 
	Uid uid = PCD_getUid();
	if (PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A , block  ,  &key ,  &uid) == mfrc522_STATUS_OK)
	{
		ret = TRUE;
	}
	return ret; 
}
static boolean Reader_WriteLoginData (loginData_Type login)
{
	boolean ret = FALSE; 
	int status1 = -1  , status2 = -1;
	if (Reader_AuthCard(ID_BLOCK) == TRUE)
	{
		status1 = PICC_WriteBlock(ID_BLOCK, login.id_w , &key) ;
	}
	
	__asm("NOP");
	
	if (Reader_AuthCard(PASS_BLOCK) == TRUE)
	{
		status2 = PICC_WriteBlock(PASS_BLOCK , login.Pass_w , &key) ; 
	}
	
	if (status1 == mfrc522_STATUS_OK && status2 == mfrc522_STATUS_OK)
		ret =  TRUE;
	
	return ret; 
}

static boolean Reader_ReadID (loginData_Type * ID)
{
	boolean ret = FALSE ;
	int status = PICC_ReadBlock(ID_BLOCK, ID->id_r  , &key ); 
	if ( status == mfrc522_STATUS_OK)
	{
		ret = TRUE; 
	}
	return ret; 
}

