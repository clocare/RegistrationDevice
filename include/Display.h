/*****************************************************************************************************************/
/* Author  : Eman ashraf                                                                                         */
/* version : v01                                                                                                 */
/* Date    :                                                                                                     */ 
/*****************************************************************************************************************/
#ifndef Display_H
#define  Display_H

#include "STD_TYPES.h"

#define LOGIN_BTN_ID     0x01
#define User_Name_Address     0x000080
#define Pass_Address          0x000400
#define Msg_Address           0x000100
#define Enable_Address        0x080000
#define Scan_Msg_Adress       0x000480
#define LOGIN_ON              0x0001
#define LOGIN_OFF             0x0000
#define LOGIN_Completed       0x01
#define LOGIN_NOT_Completed   0x02
#define READ_BTN_ID         0x03
#define VERIFY_BTN_ID         0x03
#define IS_Presented        0x01
#define VIEW_BTN_ID        0x02
//page's ids

#define Login_page_Id       0x0000
#define Scan_Add_page_Id     0x0002
#define Scan_page_Id         0x0003
#define Add_patient_page_Id   0x0004
#define View_patient_page_Id  0x0005


//patient's Data Address
#define Patient_name_Address  0x0000
#define Patient_Id_Address     0x0000
#define patient_phone_Address  0x0000
#define Patient_Age_Address   0x0000
#define Patient_intrance_Data_Address 0x0000
#define Patient_blood_type_Address  0x0000
#define Patient_Height_Address  0x0000
#define Patient_weight_Address  0x0000

/****DATA types*******************/
typedef struct {
		u8 String_Name[128];
		u8 String_ID[128];
	    u8 String_Phone[128];
		u8 String_intrance_Date[128];
	    u8 String_Blood_type[128];
       u16 PatientAge;
	  u16 PatientHeight	;
	  u16 PatientWeight	;
	}Patient_data ;


/****function ProtoTypes ************/


u8 Display_AdminLogin(void);

void Display_ScanAcard(u8  *Server_replay ) ; // server replay type need to be edited

void Display_AddPatient(void );


void Display_UserData(Patient_data *patient);
void Intalize_LcdPatientDataVP(void);
void GET_String_DATA (u8 *String1,u8 *String2);

void Display_GetWindow_Name (u8 *pageName);


#endif 


