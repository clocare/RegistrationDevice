/*****************************************************************************************************************/
/* Author  : Eman ashraf                                                                                         */
/* version : v01                                                                                                 */
/* Date    :                                                                                                     */ 
/*****************************************************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
  
#include "LCD.h"
#include "Display.h"


 




u8 Display_AdminLogin(void)
{
	u8 String_Name[128]= {0};   
	u8 String_Data1[134]= {0}; //to get reponse of read command and i need to check size 
	u8 String_Data2[134]={0};
	u8 String_Pass[134]={0};
	u8 serverreplay[100]={0};
	u8 i =2, j=2 ;
	//disable login unital verfiy username and password
	LCD_N16_write(Enable_Address,LOGIN_OFF) ;
	//write staff login 
	LCD_STR_write( Msg_Address,"Staff Login");
	// read user name  and passord 
	 
	 LCD_STR_read( User_Name_Address,String_Data1);
	 //get string data 
	 while(String_Data1[i] != '\0') 
	 {
		 String_Name[i-2]=String_Data1[i] ;
		 i++;
	 }
	 
	 LCD_STR_read( Pass_Address,String_Data2);
	 while(String_Data2[i] != '\0') 
	 {
		 String_Pass[i-2]=String_Data2[i] ;
		 j++;
	 }
	
if (LCD_GET_TouchKey_press()==VERIFY_BTN_ID &&( String_Name[0]!='\0')&&(String_Pass[0]!='\0')) 
{ 
// send data to esp through comm servises
   // ServerSendAdminLogin(String_Name,String_Pass,serverreplay);
//
if ( 1/*serverreplay== 1*/)
{
	LCD_STR_write( Msg_Address,"Verified");
	//activiate login btn 
	LCD_N16_write(Enable_Address,LOGIN_ON);
   //wait for user to press login btn 	
	while(LCD_GET_TouchKey_press!=LOGIN_BTN_ID);
		
	return LOGIN_Completed ;
	
}	
else 
{
	LCD_STR_write( Msg_Address,"error in username or password");
	LCD_N16_write(Enable_Address,LOGIN_OFF);
   return LOGIN_NOT_Completed ;
}	

	
}

	
}

void Display_ScanAcard(u8 *Server_replay){

	//u8 card_Id[127]={0};
	u8 Patient_Authitication[5]={0};
	
	//wait for  read card btn is preesed scan a card 
	while(LCD_GET_TouchKey_press==READ_BTN_ID); // i must add wait for press function 
	if (/*ScanNewCard()==*/IS_Presented)
	{
		//ReadCardId(card_Id);
		
	} //if card is present  get id 
	else 
	{
		LCD_STR_write( Scan_Msg_Adress,"please plug in card");
		
		//error msg please plug in card 
	}
	
	// send to server via esp 
	//ServerSendIdVerfication(card_Id,Patient_Authitication ,Server_replay);// in server_Replay there is patient data
	if (1/*Patient_Authitication==ok*/)
	{
	LCD_STR_write( Scan_Msg_Adress,"user found press view patient Data");
	
	// intialize lcd varabile to null 
	Intalize_LcdPAtientDataVP();
	//wait to press view patient button 
	
	while(LCD_GET_TouchKey_press==VIEW_BTN_ID);
     	
	}
	else
	{
			LCD_STR_write( Scan_Msg_Adress,"user not found press return to go to scan/new patient page");
			

	}
}

void Display_AddPatient(void )
{
	u8 server_Replay[20]={0};
	//read data entered by user 
	u8 String_Data1[134]={0};
	u8 String_Data2[134]={0};
	u8 String_Data3[134]={0};
	
	

	u8 String_Data4[134]={0};
	
	u8 String_Data5[134]={0};
	
	/*Typdef struct {
		u8 String_Name[128];
		u8 String_ID[128];
	    u8 String_Phone[128];
		u8 String_intrance_Date[128];
	    u8 String_Blood_type[128];
       u16 PatientAge;
	  u16 PatientHeight	;
	  u16 PatientWeight	;
	}Patient_data ;*/
	
	Patient_data *patient ;

	 LCD_STR_read( Patient_name_Address,String_Data1);
	 GET_String_DATA(String_Data1,patient->String_Name);
	 LCD_STR_read( Patient_Id_Address,String_Data2);
	 GET_String_DATA(String_Data2,patient->String_ID);
	 LCD_STR_read( patient_phone_Address,String_Data3);
	 GET_String_DATA(String_Data3,patient->String_Phone);
	  LCD_STR_read(Patient_intrance_Data_Address,String_Data4);
	 GET_String_DATA(String_Data4,patient->String_intrance_Date);
	 
	 
	 LCD_STR_read(Patient_blood_type_Address,String_Data5);
	 GET_String_DATA(String_Data5,patient->String_Blood_type);
	 
	patient->PatientAge= LCD_N16_read(Patient_Age_Address);
	patient->PatientHeight=LCD_N16_read(Patient_Height_Address);
	patient->PatientWeight=LCD_N16_read(Patient_weight_Address);
	
	//send Data to server 
	//if ( data != null){
		
	//ServerSendNewResgstration(patient,server_Replay);
	
	if (1/*server_Replay==ok*/ )
	{
		// msg appear to user that patient registered successfully 
	}
	else
	{
		 // msg appear to user that error occured 
	}
	
}
void Display_UserData(Patient_data *patient) // arguments pointer to struct of patient DAta 
{
	
	//DISPay Data 
	      LCD_STR_write( Patient_name_Address,patient->String_Name);
	      LCD_STR_write( Patient_Id_Address,patient->String_ID);
	      LCD_STR_write( patient_phone_Address,patient->String_Phone);
	      LCD_STR_write( Patient_intrance_Data_Address,patient->String_intrance_Date);
		  LCD_STR_write( Patient_blood_type_Address,patient->String_Blood_type);
		  LCD_N16_write( Patient_Age_Address, patient->PatientAge);
		  LCD_N16_write( Patient_Height_Address, patient->PatientHeight);
		  LCD_N16_write(Patient_weight_Address,patient->PatientWeight);
	
	
	//DISPay DAta
	
}

void Display_GetWindow_Name (u8 *pageName)
{ 
pageName=0;
	if (LCD_Read_Pg_Id()==Login_page_Id )
	{
		pageName="login page"; 
	}
	else if (LCD_Read_Pg_Id()==Scan_Add_page_Id )
	{
		pageName= "scan/add page";
	}
   else if (LCD_Read_Pg_Id()==Scan_page_Id)
   {
	pageName= "Scan page"; 	
		
   }
   else if (LCD_Read_Pg_Id()==Add_patient_page_Id)
   {
	pageName="Add page" ;	
		
   }
   else 
   {
	pageName="view page" ;	
		
   }

	
}

/*Display_GetDisp_Name (u8 *pageName)
{ 
u8 pageName=0; 
	if (LCD_GET_TouchKey_press==Login_page_Id )
	{
	
   }
   else 
   {
	pageName="view page" ;	
		
   }
   return pageName;  
	
}*/

void GET_String_DATA (u8* String1,u8* String2)
{ u8 i=2;

	while(String1[i] != '\0') 
	 {
		 String2[i-2]=String1[i] ;
		 i++;
	 }
}

void Intalize_LcdPatientDataVP(void)
{
	      LCD_STR_write( Patient_name_Address,"");
	      LCD_STR_write( Patient_Id_Address,"");
	      LCD_STR_write( patient_phone_Address,"");
	      LCD_STR_write( Patient_intrance_Data_Address,"");
		  LCD_STR_write( Patient_blood_type_Address,"");
		  LCD_N16_write( Patient_Age_Address,0x00);
		  LCD_N16_write( Patient_Height_Address,0x00);
		  LCD_N16_write(Patient_weight_Address,0x00);
	
}



