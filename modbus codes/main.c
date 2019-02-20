/* ###################################################################1    64W Annunciator with KE04Z64  With Modbus 485  

                                            WIRELESS PCB  INDOCHEM only donot have Man/Unman & Memory Backup logic        
                                            Control relay Logic is only upto 48W tested  now make it up to 64W        
 **     Filename    : main.c
 **     Project     : K04_GSM_MPU
 **     Processor   : MKE04Z64VLH4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2015-02-23, 14:05, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */         
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "rS485Port.h"
#include "ASerialLdd1.h"
#include "RS_485_Sel.h"
#include "BitIoLdd35.h"
#include "Hooter_4_Relay.h"
#include "BitIoLdd36.h"
#include "NONC_7.h"
#include "BitIoLdd37.h"
#include "NONC_8.h"
#include "BitIoLdd38.h"
#include "GROUP_7.h"
#include "BitIoLdd34.h"
#include "GROUP_8.h"
#include "BitIoLdd39.h"
#include "LATCH_7.h"
#include "BitIoLdd40.h"
#include "LATCH_8.h"
#include "BitIoLdd41.h"
#include "FAULT_SEL_7.h"
#include "BitIoLdd42.h"
#include "FAULT_SEL_8.h"
#include "BitIoLdd43.h"
#include "LEDDatabus.h"
#include "BitsIoLdd1.h"
#include "NONC_1.h"
#include "BitIoLdd1.h"
#include "NONC_2.h"
#include "BitIoLdd2.h"
#include "NONC_3.h"
#include "BitIoLdd3.h"
#include "NONC_4.h"
#include "BitIoLdd4.h"
#include "NONC_5.h"
#include "BitIoLdd5.h"
#include "NONC_6.h"
#include "BitIoLdd6.h"
#include "GROUP_1.h"
#include "BitIoLdd7.h"
#include "GROUP_2.h"
#include "BitIoLdd8.h"
#include "GROUP_3.h"
#include "BitIoLdd9.h"
#include "GROUP_4.h"
#include "BitIoLdd10.h"
#include "GROUP_5.h"
#include "BitIoLdd11.h"
#include "GROUP_6.h"
#include "BitIoLdd12.h"
#include "LATCH_1.h"
#include "BitIoLdd13.h"
#include "LATCH_2.h"
#include "BitIoLdd14.h"
#include "LATCH_3.h"
#include "BitIoLdd15.h"
#include "LATCH_4.h"
#include "BitIoLdd16.h"
#include "LATCH_5.h"
#include "BitIoLdd17.h"
#include "LATCH_6.h"
#include "BitIoLdd18.h"
#include "FAULT_SEL_1.h"
#include "BitIoLdd19.h"
#include "FAULT_SEL_2.h"
#include "BitIoLdd20.h"
#include "FAULT_SEL_3.h"
#include "BitIoLdd21.h"
#include "FAULT_SEL_4.h"
#include "BitIoLdd22.h"
#include "FAULT_SEL_5.h"
#include "BitIoLdd23.h"
#include "FAULT_SEL_6.h"
#include "BitIoLdd24.h"
#include "Hooter_1_Relay.h"
#include "BitIoLdd25.h"
#include "Hooter_2_Relay.h"
#include "BitIoLdd26.h"
#include "Hooter_3_Relay.h"
#include "BitIoLdd27.h"
#include "KEY_SELECT.h"
#include "BitIoLdd28.h"
#include "DC_FAIL_SELECT.h"
#include "BitIoLdd29.h"
#include "HEALTHY_LED.h"
#include "BitIoLdd30.h"
#include "SLAVE_ID_SELECT.h"
#include "BitIoLdd31.h"
#include "SEQUENCE_SELECT.h"
#include "BitIoLdd32.h"
#include "MAN_UNMAN_SELECT.h"
#include "BitIoLdd33.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "KeyCheck.h"



char FlashCtr, SecCtr,cCounter_4ms,cCounter_100us;
bool Flash, SecOver,FlashOver,OnTimeOver,Blink; 

//modbus Start

unsigned char Crc_HByte=0;
unsigned char Crc_LByte=0; 
unsigned int Crc =0 ;
word transmitCRC = 0;
unsigned char crcIterator=0;

unsigned int tempCRC;
typedef struct
{	
	
	unsigned char completed;
	
	unsigned char replySent;
	unsigned char timedOut;
	
	unsigned char receivedData[6];
	
	unsigned char crcLower;
	unsigned char crcHigher;
	
}FunctionCode_3_DATATYPE;

extern FunctionCode_3_DATATYPE function_3_receivedData;

typedef struct
{	
	
	unsigned char transmitData[23];  //vip  15
	
	unsigned char crcLower;
	unsigned char crcHigher;
	
}FunctionCode_3_SendDATA;

FunctionCode_3_SendDATA functionCode_3_transmitData;

typedef struct
{	
	
	unsigned char completed;
	
	unsigned char replySent;
	unsigned char timedOut;
	
	unsigned char receivedData[25];  //vip 17
	
	unsigned char crcLower;
	unsigned char crcHigher;
	
}FunctionCode_10_DATATYPE;

extern FunctionCode_10_DATATYPE function_10_receivedData;

typedef struct
{	
	
	unsigned char transmitData[8];
	
	unsigned char crcLower;
	unsigned char crcHigher;
	
}FunctionCode_10_SendDATA;

FunctionCode_10_SendDATA functionCode_10_transmitData;

extern unsigned char startingAddress;
extern unsigned char slaveAddress;


//modbus End
// FLASH/EEPROM Related declaration 
word *dataPtr ;
LDD_TError                  Error;
LDD_FLASH_TOperationStatus  OpStatus;
LDD_TDeviceData             *MF;

volatile bool  DataWrittenFlg = FALSE;


void Delay_4ms(void);
void Delay_100us(void);

//////////////////// Annunciator Declaration 
 
void ScanKeyBoard(void);
void delay(void);
void latchout(void);
void readnonc(void);
void readgroup(void);
void Scan_Seq_Dip(void);
void GetSlaveAddress(void);  
void GetDCFail_Window(void);
void testflash(void);
void keyaction(void);
void ManualFun(void);
void autmanreadfaults(void);
void RelayOutput(void);
void Relay_2_Output(void);

void autseq(void);
void ringback(void);
void rgbkreadfaults(void);
void ScanFault(void);
void fault_firstup(void);
void fstupreadfaults(void);
void manual1(void);
void key_pressed(void);            
void DIP_key_pressed(void);        
  
void MSDelay(unsigned int itime); //vipul

/////////////////    Annunciator Declaration End


////////////////  Annunciator Variables     /////////////////////////
//unsigned int portstatus;
unsigned char portstatus;
unsigned char mcnt;
unsigned char FTmp[50];
unsigned char SEC1_REG,SEC2_REG,delay_2ms,tmp_, SEC5_REG;
unsigned char higher;
unsigned char window,window1,window2,window3,window4,window5,window6,window7;
unsigned char noncstatus1,noncstatus2,noncstatus3,noncstatus4,noncstatus5,noncstatus6,noncstatus7,noncstatus8;
unsigned char groupstatus1,groupstatus2,groupstatus3,groupstatus4,groupstatus5,groupstatus6,groupstatus7,groupstatus8;
unsigned char fltstatus1to8,fltstatus9to16,fltstatus17to24,fltstatus25to32,fltstatus33to40,fltstatus41to48,fltstatus49to56,fltstatus57to64;
unsigned char steadystatus1,steadystatus2,steadystatus3,steadystatus4,steadystatus5,steadystatus6,steadystatus7,steadystatus8;
unsigned char f_pinstatus1to8,f_pinstatus9to16,f_pinstatus17to24,f_pinstatus25to32,f_pinstatus33to40,f_pinstatus41to48,f_pinstatus49to56,f_pinstatus57to64;
unsigned char fltstatus1to8n,fltstatus9to16n,fltstatus17to24n,fltstatus25to32n,fltstatus33to40n,fltstatus41to48n,fltstatus49to56n,fltstatus57to64n;
unsigned char fltstatus1to8new1,fltstatus9to16new2,fltstatus17to24new3,fltstatus25to32new4,fltstatus33to40new5,fltstatus41to48new6,fltstatus49to56new7,fltstatus57to64new8;
unsigned char fltstatus1to8new,fltstatus9to16new,fltstatus17to24new,fltstatus25to32new,fltstatus33to40new,fltstatus41to48new,fltstatus49to56new,fltstatus57to64new;  
unsigned char flt1,flt2,flt3,flt4,flt5,flt6,flt7,flt8;
unsigned char andingR1a,andingR1b,andingR1c,andingR1d,andingR1e,andingR1f,andingR1g,andingR1h;     
unsigned char andingR2a,andingR2b,andingR2c,andingR2d,andingR2e,andingR2f,andingR2g,andingR2h;
unsigned char windowa,windowb,window1a,window1b,window2a,window2b,window3a,window3b,window4a,window4b,window5a,window5b,window6a,window6b,window7a,window7b;
unsigned char fltstatusrgbk1to8,fltstatusrgbk9to16,fltstatusrgbk17to24,fltstatusrgbk25to32,fltstatusrgbk33to40,fltstatusrgbk41to48,fltstatusrgbk49to56,fltstatusrgbk57to64;
unsigned char fltstatusnw1to8,fltstatusnw9to16,fltstatusnw17to24,fltstatusnw25to32,fltstatusnw33to40,fltstatusnw41to48,fltstatusnw49to56,fltstatusnw57to64; 
unsigned char fltstatusclr1to8, fltstatusclr9to16, fltstatusclr17to24,fltstatusclr25to32,fltstatusclr33to40,fltstatusclr41to48,fltstatusclr49to56,fltstatusclr57to64;
unsigned char	fltstatusrav1to8,fltstatusrav9to16,fltstatusrav17to24,fltstatusrav25to32,fltstatusrav33to40,fltstatusrav41to48,fltstatusrav49to56,fltstatusrav57to64;
unsigned char Firstup_flt1to8,Firstup_flt9to16,Firstup_flt17to24,Firstup_flt25to32,Firstup_flt33to40,Firstup_flt41to48,Firstup_flt49to56,Firstup_flt57to64;
unsigned char Firstup_std1to8,Firstup_std9to16,Firstup_std17to24,Firstup_std25to32,Firstup_std33to40,Firstup_std41to48,Firstup_std49to56,Firstup_std57to64;
unsigned char store1,store2,store3,store4,store5,store6,store7,store8;
unsigned char Silence1,Silence2,Silence3,Silence4,Silence5,Silence6,Silence7,Silence8;
unsigned char Silent_status1to8,Silent_status9to16,Silent_status17to24,Silent_status25to32,Silent_status33to40,Silent_status41to48,Silent_status49to56,Silent_status57to64;
unsigned char cnt; 
unsigned char key,DIP_Key;
unsigned char Write_key=0;



bool TEST,ACCEPT,RSET,SILENCE;
bool MANUAL,AUTO_K,FIRST_UP,RING_BAK;
bool flashflag,acctest,TESTMODE,clear,accflt,AcceptFlag,accrgbk,newfaultflag;
bool resetrgbkflag,flashflag1,accflag,resetfstupflag,TxFlag;
bool FirstFault=0;
bool ANNTEST,ANNTESTACCEPT,ANNACCEPT,ANNRESET,ANNFAULT,RESETEST;
bool PCTEST,PCACCEPT,PCRESET,PCTESTACCEPT,Hooter3Fault;
bool Sil,Sil1,SilenceFlg;
bool Healthy=1;

bool Five_Second_Over;


unsigned char Power_ON_flag,SEC3_REG;   
unsigned char Annun_Fail_flag=0;  
unsigned char DCFail_no,Hooter_3_Flag;

/*


typedef union
	{
	struct {			             //TO DEFINE FLAGS REGS TYPE
	unsigned	bit0 : 1 ;
	unsigned	bit1 : 1 ;
	unsigned	bit2 : 1 ;
	unsigned	bit3 : 1 ;
	unsigned	bit4 : 1 ;
	unsigned	bit5 : 1 ;
	unsigned	bit6 : 1 ;
	unsigned	bit7 : 1 ;
	};
	unsigned char DataByte;
} bit_regs ;

 bit_regs  dummyUnion;


*/

unsigned char window_New_Pattern,window_1_New_Pattern,window_2_New_Pattern,window_3_New_Pattern,window_4_New_Pattern,window_5_New_Pattern,window_6_New_Pattern,window_7_New_Pattern;
unsigned char New_window_1,New_window_2,New_window_3,New_window_4,New_window_5,New_window_6,New_window_7,New_window_8;
unsigned char New_window_9,New_window_10,New_window_11,New_window_12,New_window_13,New_window_14,New_window_15,New_window_16;
unsigned char New_window_17,New_window_18,New_window_19,New_window_20,New_window_21,New_window_22,New_window_23,New_window_24; 
unsigned char New_window_25,New_window_26,New_window_27,New_window_28,New_window_29,New_window_30,New_window_31,New_window_32; 
unsigned char New_window_33,New_window_34,New_window_35,New_window_36,New_window_37,New_window_38,New_window_39,New_window_40; 
unsigned char New_window_41,New_window_42,New_window_43,New_window_44,New_window_45,New_window_46,New_window_47,New_window_48; 
unsigned char New_window_49,New_window_50,New_window_51,New_window_52,New_window_53,New_window_54,New_window_55,New_window_56;
unsigned char New_window_57,New_window_58,New_window_59,New_window_60,New_window_61,New_window_62,New_window_63,New_window_64;

//////////////////////////////// Annunciator Variables End ////////////////////////////////////////////////////////////////////////////








                ///////////////////////////////   24W Logic Start   ////////////////////////////////

 void delay(void)
 {
	 for(delay_2ms=0;delay_2ms<48;delay_2ms++)	//48
	 {}
 }
 
/////////////////////////////////////////////////////////

void ScanKeyBoard(void)
{
	  unsigned char temp;
		KEY_SELECT_ClrVal();          //keysel=0; 
		LEDDatabus_SetDir(0);         //P0=0XFF;  //DATAPORT as Input   
		delay();
		delay();
		do 
		{
		    portstatus = (LEDDatabus_GetVal()) & 0X0F ;
		} while (portstatus != ((LEDDatabus_GetVal()) & 0X0F)) ;
		KEY_SELECT_SetVal();         //keysel=1;
		delay();
		temp=~(portstatus&0x0F);
		key = temp & 0x0F;	
}

////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////	
////////////////////////////////	24W Logic End   ///////////////////////////
void resetFuction3ReceivedFrame(void){
	unsigned char iterator=0;
	
	function_3_receivedData.completed=FALSE; 
	function_3_receivedData.replySent=TRUE;
	function_3_receivedData.timedOut=FALSE;
	
	function_3_receivedData.crcHigher = 0;
	function_3_receivedData.crcLower = 0;
	
	for(iterator = 0; iterator < 6; iterator++){
		function_3_receivedData.receivedData[iterator] = 0;
	}
}

void resetFuction10ReceivedFrame(void){
	unsigned char iterator=0;
	
	function_10_receivedData.completed=FALSE; 
	function_10_receivedData.replySent=TRUE;
	function_10_receivedData.timedOut=FALSE;
	
	function_10_receivedData.crcHigher = 0;
	function_10_receivedData.crcLower = 0;
	
	for(iterator = 0; iterator < 25; iterator++){   //vip 17
		function_10_receivedData.receivedData[iterator] = 0;
	}
}

void processFunction10ReceivedData(void){
	
	
//							     fltstatus1to8= function_10_receivedData.receivedData[8];
//							     fltstatus9to16=function_10_receivedData.receivedData[7];
//							     fltstatus17to24=function_10_receivedData.receivedData[10];
//							     fltstatus25to32=function_10_receivedData.receivedData[9];
//							     fltstatus33to40=function_10_receivedData.receivedData[12];
//							     fltstatus41to48=function_10_receivedData.receivedData[11];

//								 fltstatus1to8= function_10_receivedData.receivedData[8]   | fltstatus1to8;
//							     fltstatus9to16=function_10_receivedData.receivedData[7]   | fltstatus9to16;
//							     fltstatus17to24=function_10_receivedData.receivedData[10] | fltstatus17to24;
//							     fltstatus25to32=function_10_receivedData.receivedData[9]  | fltstatus25to32 ;
//							     fltstatus33to40=function_10_receivedData.receivedData[12] | fltstatus33to40;
//							     fltstatus41to48=function_10_receivedData.receivedData[11] | fltstatus41to48;   
//							     fltstatus49to56=function_10_receivedData.receivedData[14] | fltstatus49to56;
//							     fltstatus57to64=function_10_receivedData.receivedData[13] | fltstatus57to64;
//	
//							     Write_key = function_10_receivedData.receivedData[15];
							     
							     
							     fltstatus1to8= function_10_receivedData.receivedData[8]   | fltstatus1to8;
							     fltstatus9to16=function_10_receivedData.receivedData[7]   | fltstatus9to16;
							     fltstatus17to24=function_10_receivedData.receivedData[10] | fltstatus17to24;
							     fltstatus25to32=function_10_receivedData.receivedData[9]  | fltstatus25to32 ;
							     fltstatus33to40=function_10_receivedData.receivedData[12] | fltstatus33to40;
							     fltstatus41to48=function_10_receivedData.receivedData[11] | fltstatus41to48;   
							     fltstatus49to56=function_10_receivedData.receivedData[14] | fltstatus49to56;
							     fltstatus57to64=function_10_receivedData.receivedData[13] | fltstatus57to64;
							     
							     steadystatus1= function_10_receivedData.receivedData[16]   | steadystatus1;
							     steadystatus2=function_10_receivedData.receivedData[15]    | steadystatus2;
							     steadystatus3=function_10_receivedData.receivedData[18]   | steadystatus3;
							     steadystatus4=function_10_receivedData.receivedData[17]    | steadystatus4 ;
							     steadystatus5=function_10_receivedData.receivedData[20]   | steadystatus5;
							     steadystatus6=function_10_receivedData.receivedData[19]   | steadystatus6;   
							     steadystatus7=function_10_receivedData.receivedData[22]   | steadystatus7;
							     steadystatus8=function_10_receivedData.receivedData[21]   | steadystatus8;
	
							     Write_key = function_10_receivedData.receivedData[23];
	
}
// CRC16 calculation
// ----------------
unsigned int CRC16(unsigned int crc, unsigned int data)
{
 const unsigned int Poly16=0xA001;
 unsigned int LSB, i;

 crc = ((crc^data) | 0xFF00) & (crc | 0x00FF);
 for (i=0; i<8; i++) {
 LSB=(crc & 0x0001);
 crc=crc/2;
 if (LSB)
 crc=crc^Poly16;
 }
 return(crc);
} 


void update_checksum (unsigned char b)
{
	uint16_t xx;
	unsigned char i = 0;                     
	
    	xx = 0x00ff & b;
    	transmitCRC = transmitCRC ^ xx;
    	for (i = 0; i < 8; i++)
    	{
    		if (transmitCRC & 0x0001)
    		{
    			transmitCRC = transmitCRC >> 1;
    			xx = 0xa001;
    			transmitCRC = transmitCRC ^ xx;
    		}
    		else
    			transmitCRC = transmitCRC >> 1;
    	}
	
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */


{
	/* Write your local variable definition here */
	
	unsigned char tempLowerCRC=0,tempHigherCRC=0;
	
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */
	ICS_C3=0x80; 
	
	
	tempLowerCRC=0;
	tempHigherCRC=0;
	
	////////////////////////  Annunciator Related  ///////
        
   function_3_receivedData.completed=FALSE;
   function_3_receivedData.replySent=FALSE;
   function_3_receivedData.timedOut=FALSE;
     
     MSDelay(150);    // Power ON initialise delay 
             
     RS_485_Sel_ClrVal(); //rs 485 as input
     
	   for (;;)
	   {   

		   
		   //modbus Start
		   
		   
		   if(        function_3_receivedData.completed==TRUE 
				   && function_3_receivedData.replySent==FALSE 
				   && function_3_receivedData.timedOut==FALSE
			  ){
					   Crc=0xFFFF;
					   
					   for (crcIterator = 0; crcIterator < 6 ; crcIterator++) {
						   Crc = CRC16 (Crc, function_3_receivedData.receivedData[crcIterator] );
					   }
					   
					   Crc_LByte =(unsigned char) (Crc & 0x00FF); // Low byte calculation
					   Crc_HByte =(unsigned char)( (Crc & 0xFF00) / 256); // High byte calculation
					   

                          
	                   if( function_3_receivedData.crcLower==Crc_LByte && function_3_receivedData.crcHigher==Crc_HByte)
					   {

						transmitCRC =  0xffff;
						     
						     functionCode_3_transmitData.transmitData[0]=slaveAddress;	
						     update_checksum( functionCode_3_transmitData.transmitData[0]);
						     
						     functionCode_3_transmitData.transmitData[1]=3;//function code
						     update_checksum( functionCode_3_transmitData.transmitData[1]);

						     functionCode_3_transmitData.transmitData[2]=18;//byte count  //vip10
						     update_checksum( functionCode_3_transmitData.transmitData[2]);
						     
						     functionCode_3_transmitData.transmitData[3]=fltstatus9to16;
						     update_checksum( functionCode_3_transmitData.transmitData[3]);
						     
						     functionCode_3_transmitData.transmitData[4]=fltstatus1to8;
						     update_checksum( functionCode_3_transmitData.transmitData[4]);
						     
						     functionCode_3_transmitData.transmitData[5]=fltstatus25to32;
						     update_checksum( functionCode_3_transmitData.transmitData[5]);
						     
						     functionCode_3_transmitData.transmitData[6]=fltstatus17to24;
						     update_checksum( functionCode_3_transmitData.transmitData[6]);
						     
						     functionCode_3_transmitData.transmitData[7]=fltstatus41to48;
						     update_checksum( functionCode_3_transmitData.transmitData[7]);
						     
						     functionCode_3_transmitData.transmitData[8]=fltstatus33to40;
						     update_checksum( functionCode_3_transmitData.transmitData[8]);
						     
						     functionCode_3_transmitData.transmitData[9]=fltstatus57to64;
						     update_checksum( functionCode_3_transmitData.transmitData[9]);


						     functionCode_3_transmitData.transmitData[10]=fltstatus49to56;
						     update_checksum( functionCode_3_transmitData.transmitData[10]);
						     
						     
						     
						     functionCode_3_transmitData.transmitData[11]=steadystatus2;      //9 to 16 
						     update_checksum( functionCode_3_transmitData.transmitData[11]);
						     
						     functionCode_3_transmitData.transmitData[12]=steadystatus1;      //1 to 8
						     update_checksum( functionCode_3_transmitData.transmitData[12]);
						     
						     functionCode_3_transmitData.transmitData[13]=steadystatus4;   //25 to 32
						     update_checksum( functionCode_3_transmitData.transmitData[13]);
						     
						     functionCode_3_transmitData.transmitData[14]=steadystatus3;   //17 to 24
						     update_checksum( functionCode_3_transmitData.transmitData[14]);
						     
						     functionCode_3_transmitData.transmitData[15]=steadystatus6;   //41 to 48
						     update_checksum( functionCode_3_transmitData.transmitData[15]);
						     
						     functionCode_3_transmitData.transmitData[16]=steadystatus5;   //33 to 40
						     update_checksum( functionCode_3_transmitData.transmitData[16]);
						     
						     functionCode_3_transmitData.transmitData[17]=steadystatus8;   //57 to 64
						     update_checksum( functionCode_3_transmitData.transmitData[17]);

						     functionCode_3_transmitData.transmitData[18]=steadystatus7;   //49 to 56
						     update_checksum( functionCode_3_transmitData.transmitData[18]);
						     
						     
						     
						     
						     functionCode_3_transmitData.transmitData[19]=Write_key;//test accept reset silence byte//test being LSB
						     update_checksum( functionCode_3_transmitData.transmitData[19]);
						     
						     functionCode_3_transmitData.transmitData[20]=0;//dummy byte
						     update_checksum( functionCode_3_transmitData.transmitData[20]);
						    
	/*					     
						     Crc=0xFFFF;
						     					   
						     					   for (crcIterator = 0; crcIterator < 11 ; crcIterator++) {
						     						   Crc = CRC16 (Crc, functionCode_3_transmitData.transmitData[crcIterator] );
						     					   }
						     					   
						     					   Crc_LByte = (Crc & 0x00FF); // Low byte calculation
						     					   Crc_HByte = (Crc & 0xFF00) / 256; // High byte calculation*/
						     					   
						     					   
						     functionCode_3_transmitData.transmitData[21]=(transmitCRC & 0x00ff);
						     functionCode_3_transmitData.transmitData[22]=( transmitCRC >> 8);
						     
						     RS_485_Sel_SetVal();
												     delay485Long();
												  
												   for (crcIterator = 0; crcIterator < 23 ; crcIterator++) {   //vip 15
														   rS485Port_SendChar(functionCode_3_transmitData.transmitData[crcIterator] );
														   delay485();
												   }
												   
					
						   
											resetFuction3ReceivedFrame();
												   
								RS_485_Sel_ClrVal();
								
					   }
					   else{
						   resetFuction3ReceivedFrame();
					   }
		   }
		   
		   if(        function_10_receivedData.completed==TRUE 
						   && function_10_receivedData.replySent==FALSE 
						   && function_10_receivedData.timedOut==FALSE
					  ){
			   
			   Crc=0xFFFF;
			   					   
			   					   for (crcIterator = 0; crcIterator < 25 ; crcIterator++) {  //vip 17
			   						   Crc = CRC16 (Crc, function_10_receivedData.receivedData[crcIterator] );
			   					   }
			   					   
			   					   Crc_LByte = (Crc & 0x00FF); // Low byte calculation
			   					   Crc_HByte = (Crc & 0xFF00) / 256; // High byte calculation
			   					   
			   					   if(function_10_receivedData.crcLower==Crc_LByte && function_10_receivedData.crcHigher==Crc_HByte)
			   					   {
			   						   
			   						   processFunction10ReceivedData();
			   						   
			   						RS_485_Sel_SetVal();
			   						delay485Long();
			   						
			   						transmitCRC =  0xffff;

			   						functionCode_10_transmitData.transmitData[0]=slaveAddress;	

			   						functionCode_10_transmitData.transmitData[1]=16;//function code
			   						
			   						functionCode_10_transmitData.transmitData[2]=0;// 

			   						functionCode_10_transmitData.transmitData[3]=	function_10_receivedData.receivedData[3];// 

			   						functionCode_10_transmitData.transmitData[4]=0;// 

			   						functionCode_10_transmitData.transmitData[5]=9;// length  5  //vip
			   						
			   					  Crc=0xFFFF;
			   											     					   
									   for (crcIterator = 0; crcIterator < 6 ; crcIterator++) {
										   Crc = CRC16 (Crc, functionCode_10_transmitData.transmitData[crcIterator] );
									   }
									   
									   Crc_LByte = (Crc & 0x00FF); // Low byte calculation
									   Crc_HByte = (Crc & 0xFF00) / 256; // High byte calculation
			   											     					   
			   				       functionCode_10_transmitData.transmitData[6]=Crc_LByte;
			   					   functionCode_10_transmitData.transmitData[7]=Crc_HByte;
			   						
			   					   for (crcIterator = 0; crcIterator < 8 ; crcIterator++) {
												   rS485Port_SendChar(functionCode_10_transmitData.transmitData[crcIterator] );
												   delay485();
										   }
			   																	   
			   					   
			   						RS_485_Sel_ClrVal();
			   						resetFuction10ReceivedFrame();  
			   					   }
			   					   else{
			   						resetFuction10ReceivedFrame();
			   					   }
			   
		   }
		   
	
		   
		   
		   
		   //modbus End
	   }





	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.3 [05.09]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */
