
/*16x2 character lcd
 * author: Suraj shinde
 */

#pragma config OSC=INTIO67, LVP=OFF, WDT=OFF, PBADEN=OFF    //config bits


#include<xc.h>                                              //compiler library
#include<p18f4520.h>                                        //controller library
#include<string.h>                                          /*by avoiding just strlen function program can save addition of extra header file
                                                             it will not affect the final hex size though,
                                                             but it's better to use user defined function for string length */
#include<stdio.h>                                           /*sprintf derived from here*/

#define rs PORTCbits.RC0                                    //LCD pins
#define rw PORTCbits.RC1
#define en PORTCbits.RC2

void lcd_init();                                            //LCD functions
void lcd_data(unsigned char data);
void lcd_cmd(unsigned char cmd);
void strobe();
void delay(unsigned int time);

unsigned char dat[13]="ADC Value:  ";                     //strings to show on LCD
unsigned char vol[13]="Voltage is: ";                     //don't mention size of strings 
int k,ADC_Value;                                        
unsigned char ADC_Array[5];
int value;
unsigned char buffer[20];                               //sprintf buffer

void main(void) {
    
    TRISCbits.TRISC0=0;             //rs
    TRISCbits.TRISC1=0;             //rw
    TRISCbits.TRISC2=0;             //en
    
    TRISA=0XFF;                     //adc channel as input
    TRISD=0x00;                     //LCd port
    PORTD=0x00;
    delay(40);
    
    ADCON0	=	0x01;              //go_done bit should not be set in the same instruction as of ADON=1 !!!
    /*ADCON0 mapping
     bit0: ADON is bit for turning on or off ADC module
     bit1: GO/DONE bit for conversion
     bit2-5: channel select bit for ADC
     bit6-7: unimplemented*/
    
    ADCON1	=	0x0d;                       //AN0 and AN1 as analog, others digital
    /*ADCON1 mapping
     bit0-3:bits for analog and digital channel selection
     bit4: VREF+ source, select 0 for VDD
     bit5: VREF- source, select 0 for VSS
     bit6-7: unimplemented
     */
    
    ADCON2	=	0x91;                                         //ADCS FOSC/8 or TOSC*8 and ACQT 4TAD
    /*bit0-2: Oscillator selection bit. selects conversion time per bit i.e TAD
              min TAD should be 0.7us as per datasheet
     bit3-5: acquisition time select bits, minimum acquisition time should be more than 2.4us
     bit7: select 1 for right justified, bits will stick to the right...*/
    
    en=0;
    rw=0;
    lcd_init();                     //LCD initialization routine
    for(int i=0; i<13;i++)          //print text "ADC value:" on LCD, alternatively sprintf can also be used
        lcd_data(dat[i]);
    
    while(1)
     {
    
      ADCON0bits.GO=1;                    /* Set A/D conversion status bit( conversion in progress) */
 	  while(ADCON0bits.GO==1);             /* Wait until A\D conversion completed               */
      ADC_Value= ADRESH;                   /* Store the 2 bit in ADRESH to the variable ADC_Value */
      ADC_Value=ADC_Value<<8;
      ADC_Value=ADC_Value+ADRESL;          /* 8 times left shifted ADRESH value is added to 8 bit ADRESL*/
   
	  for(k=0;k<=3;k++)
       { 
		  ADC_Array[k]=ADC_Value%10+'0';   /* Convert the result into ASCII 
                                           binary remainder decimal 10 returns a decimal value*/
		  ADC_Value=ADC_Value/10;
	   }
    
      lcd_cmd(0x8C);                        //line 1, posion 12
      
      for(k=3;k>=0;k--)
      {
		lcd_data(ADC_Array[k]);            /* Display the result on LCD    */
        
	  }
      
      value=0;                             //convert data to integer from ASCII
      value+=(ADC_Array[3]-'0')*1000;
      value+=(ADC_Array[2]-'0')*100;
      value+=(ADC_Array[1]-'0')*10;
      value+=(ADC_Array[0]-'0');
      
      lcd_cmd(0xC0);                        //line 2, position 0
      
      sprintf(buffer,"Voltage:  %.2f",(value/1023.0)*5);       //an easy alternative to convert data to string
                                                               //but one should think of memory footprint of string.h library
      
      for(int i=0;i<strlen(buffer);i++)
          lcd_data(buffer[i]);    
    }
    
    return;
}

void lcd_init()
{
    lcd_cmd(0x38);                 //2 line, 8 bit, 5x7 dots
    delay(30);
    //lcd_cmd(0x0F);               //display on, cursor blink
    lcd_cmd(0x0C);                 //display on, cursor off
    delay(30);
    lcd_cmd(0x01);                 //clear lcd
    delay(30);
    lcd_cmd(0x06);                 //Entry mode, auto increment with no shift
    delay(30);
    
}

void lcd_cmd(unsigned char cmd)
{
    rs=0;
    PORTD= cmd;
    strobe();
}

void lcd_data(unsigned char data)
{
    rs=1;
    PORTD= data;
    strobe();
}

void strobe()
{
    en=1;
    delay(20);
    en=0;
}

void delay(unsigned int time)
{
   unsigned int i,j;
   for(i=0;i<time;i++)
       for(j=0;j<165;j++);
}
