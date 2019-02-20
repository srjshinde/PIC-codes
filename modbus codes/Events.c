/* ###################################################################
 **     Filename    : Events.c
 **     Project     : K04_GSM_MPU
 **     Processor   : MKE04Z64VLH4
 **     Component   : Events
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2015-02-23, 14:05, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Settings    :
 **     Contents    :
 **         Cpu_OnNMI - void Cpu_OnNMI(void);
 **
 ** ###################################################################*/
/*!
 ** @file Events.c
 ** @version 01.00
 ** @brief
 **         This is user's event module.
 **         Put your event handler code here.
 */         
/*!
 **  @addtogroup Events_module Events module documentation
 **  @{
 */         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"
//#include "KeyCheck.h"


char mSec50;
extern char FlashCtr, SecCtr;
extern bool Flash, SecOver,FlashOver,Blink;
char ms_8;
extern volatile bool DataWrittenFlg;
unsigned short rxDataPointer;
unsigned char RX_DATA_BUFFER[1024];
rS485Port_TError *tempERR;

unsigned char startingAddress=0;
unsigned char slaveAddress=1;

typedef struct
{	
	
	unsigned char completed;
	
	unsigned char replySent;
	unsigned char timedOut;
	
	unsigned char receivedData[6];
	
	unsigned char crcLower;
	unsigned char crcHigher;
	
}FunctionCode_3_DATATYPE;

FunctionCode_3_DATATYPE function_3_receivedData;

typedef struct
{	
	
	unsigned char completed;
	
	unsigned char replySent;
	unsigned char timedOut;
	
	unsigned char receivedData[25];  //vip 17
	
	unsigned char crcLower;
	unsigned char crcHigher;
	
}FunctionCode_10_DATATYPE;

FunctionCode_10_DATATYPE function_10_receivedData;

void TI1_OnInterrupt(void)
{

	mSec50++;
	if (mSec50 >= 12)
	 {
		 mSec50 = 0;
		 FlashCtr++;

		 if (FlashCtr >= 5)
		 {
			FlashCtr = 0;
			FlashOver= TRUE;
		 }
		
		SecCtr++;
		
		 if (SecCtr >= 20)  
		 {
			SecCtr = 0;
			SecOver = TRUE;
			Blink=!Blink;
			Flash = !Flash;            
		 }

	 }
}


/* ===================================================================*/
void FLASH1_OnOperationComplete(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
	DataWrittenFlg = TRUE;
}
/*
** ===================================================================
**     Event       :  rS485Port_OnError (module Events)
**
**     Component   :  rS485Port [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void rS485Port_OnError(void)
{
  /* Write your code here ... */
	
	rS485Port_GetError(&tempERR);
}

/*
** ===================================================================
**     Event       :  rS485Port_OnRxChar (module Events)
**
**     Component   :  rS485Port [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void rS485Port_OnRxChar(void)
{
	rS485Port_RecvChar(&RX_DATA_BUFFER[rxDataPointer]);
	
	rxDataPointer++;
	
	if(        
		     RX_DATA_BUFFER[rxDataPointer-8]==slaveAddress 
		  && RX_DATA_BUFFER[rxDataPointer-7]==3
			&& RX_DATA_BUFFER[rxDataPointer-6]==0 && (RX_DATA_BUFFER[rxDataPointer-5]==0 || RX_DATA_BUFFER[rxDataPointer-5]==1)  //start address 0 or 1
			&& RX_DATA_BUFFER[rxDataPointer-4]==0 && RX_DATA_BUFFER[rxDataPointer-3]==9   // length =5
			//&& RX_DATA_BUFFER[rxDataPointer-2]==68 && RX_DATA_BUFFER[rxDataPointer-1]==9
			&& function_3_receivedData.completed==FALSE)
	{
		function_3_receivedData.completed=TRUE;
		function_3_receivedData.replySent=FALSE;
		function_3_receivedData.timedOut=FALSE;
		
		function_3_receivedData.receivedData[0]= RX_DATA_BUFFER[rxDataPointer-8];
		function_3_receivedData.receivedData[1]= RX_DATA_BUFFER[rxDataPointer-7];
		function_3_receivedData.receivedData[2]= RX_DATA_BUFFER[rxDataPointer-6];
		function_3_receivedData.receivedData[3]= RX_DATA_BUFFER[rxDataPointer-5];
		function_3_receivedData.receivedData[4]= RX_DATA_BUFFER[rxDataPointer-4];
		function_3_receivedData.receivedData[5]= RX_DATA_BUFFER[rxDataPointer-3];
		
		function_3_receivedData.crcLower  =  RX_DATA_BUFFER[rxDataPointer-2];
		function_3_receivedData.crcHigher =  RX_DATA_BUFFER[rxDataPointer-1];
	}
	
	if(        
			     RX_DATA_BUFFER[rxDataPointer-27]==slaveAddress 
			  && RX_DATA_BUFFER[rxDataPointer-26]==16
				&& RX_DATA_BUFFER[rxDataPointer-25]==0 
				&& (RX_DATA_BUFFER[rxDataPointer-24]==0  || RX_DATA_BUFFER[rxDataPointer-24]==1)  //start address 0 or 1
				&& RX_DATA_BUFFER[rxDataPointer-23]==0 
				&& RX_DATA_BUFFER[rxDataPointer-22]==9 //length =5  //vip
				&& RX_DATA_BUFFER[rxDataPointer-21]==18 //byte count //vip 10
				&& RX_DATA_BUFFER[rxDataPointer-3]==0
				&& function_10_receivedData.completed==FALSE){
		
				function_10_receivedData.completed=TRUE;
				function_10_receivedData.replySent=FALSE;
				function_10_receivedData.timedOut=FALSE;
				
				function_10_receivedData.receivedData[0]=RX_DATA_BUFFER[rxDataPointer-27];

				function_10_receivedData.receivedData[1]=RX_DATA_BUFFER[rxDataPointer-26];
				function_10_receivedData.receivedData[2]=RX_DATA_BUFFER[rxDataPointer-25];
				function_10_receivedData.receivedData[3]=RX_DATA_BUFFER[rxDataPointer-24];
				function_10_receivedData.receivedData[4]=RX_DATA_BUFFER[rxDataPointer-23];
		
				function_10_receivedData.receivedData[5]=RX_DATA_BUFFER[rxDataPointer-22];
				function_10_receivedData.receivedData[6]=RX_DATA_BUFFER[rxDataPointer-21];
				function_10_receivedData.receivedData[7]=RX_DATA_BUFFER[rxDataPointer-20];
				
				function_10_receivedData.receivedData[8]=RX_DATA_BUFFER[rxDataPointer-19];
				function_10_receivedData.receivedData[9]=RX_DATA_BUFFER[rxDataPointer-18];
				function_10_receivedData.receivedData[10]=RX_DATA_BUFFER[rxDataPointer-17];
				function_10_receivedData.receivedData[11]=RX_DATA_BUFFER[rxDataPointer-16];
				
				
				function_10_receivedData.receivedData[12]=RX_DATA_BUFFER[rxDataPointer-15];
				function_10_receivedData.receivedData[13]=RX_DATA_BUFFER[rxDataPointer-14];
				function_10_receivedData.receivedData[14]=RX_DATA_BUFFER[rxDataPointer-13];

				function_10_receivedData.receivedData[15]=RX_DATA_BUFFER[rxDataPointer-12];
				function_10_receivedData.receivedData[16]=RX_DATA_BUFFER[rxDataPointer-11];
				
				function_10_receivedData.receivedData[17]=RX_DATA_BUFFER[rxDataPointer-10];
				function_10_receivedData.receivedData[18]=RX_DATA_BUFFER[rxDataPointer-9];
				function_10_receivedData.receivedData[19]=RX_DATA_BUFFER[rxDataPointer-8];
				function_10_receivedData.receivedData[20]=RX_DATA_BUFFER[rxDataPointer-7];
				function_10_receivedData.receivedData[21]=RX_DATA_BUFFER[rxDataPointer-6];
				function_10_receivedData.receivedData[22]=RX_DATA_BUFFER[rxDataPointer-5];
				function_10_receivedData.receivedData[23]=RX_DATA_BUFFER[rxDataPointer-4];
				function_10_receivedData.receivedData[24]=RX_DATA_BUFFER[rxDataPointer-3];
				
				function_10_receivedData.crcLower =  RX_DATA_BUFFER[rxDataPointer-2];
				function_10_receivedData.crcHigher =  RX_DATA_BUFFER[rxDataPointer-1];
				
				
	}
	
	
	if(rxDataPointer>1022)
		rxDataPointer=0;
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  rS485Port_OnTxChar (module Events)
**
**     Component   :  rS485Port [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void rS485Port_OnTxChar(void)
{
  /* Write your code here ... */
}


