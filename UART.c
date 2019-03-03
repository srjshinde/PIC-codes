#pragma config OSC=INTIO67, LVP=OFF, WDT=OFF, PBADEN=OFF,PWRT=ON    //config bits
#include<pic18.h>        

#define rs PORTCbits.RC0
#define rw PORTCbits.RC1
#define en PORTCbits.RC2

void send2uart( char *data);
void lcd_init(void);
void lcd_data(char data);
void lcd_cmd(char cmd);
void strobe(void);
void delay(unsigned int time);
void send2lcd(char *lcd_string);

void main()                            
{
    
 OSCCONbits.IRCF0=1;            //set the oscillator to 8MHz
 OSCCONbits.IRCF1=1;
 OSCCONbits.IRCF2=1;
 
 TRISCbits.TRISC0=0;             //rs
 TRISCbits.TRISC1=0;             //rw
 TRISCbits.TRISC2=0;             //wn
 TRISD=0x00;
 PORTD=0x00;
    
 en=0;
 rw=0;
 lcd_init();

 TRISCbits.TRISC6 = 0;         /* RC6=0 (O/P) RC7=1(I/P)                      */
 TRISCbits.TRISC7 = 1;         /* RC6=0 (O/P) RC7=1(I/P)                      */
 
 SPBRG = 0x33;         /* Serial Port Baud Rate Generator for 9600 ,8MHz   */  
 TXSTA = 0X24;         /* TXEN=1, SYNC=0, BRGH=1                      */
 RCSTA = 0X90;         /* Reception Enable (SPEN=1,CREN=1)            */
 
 send2uart("welcome to the orbit!!! \r\n");
 
 while(1)
 {
  if(RCIF==1)
  {
   RCIF=0;             /* Clear Receive interrupt flag bit            */
   //if (RCREG != 0x12 && RCREG != 0x15 && RCREG != 0x26 )  //well, this is useless!!
   if (RCREG != '\r' && RCREG != '\n' /*&& RCREG != '\r\n' <-- and this one too!!!*/)
   {   lcd_data(RCREG);
       delay(20);
   }
  }
 }
}
 
void send2uart( char *data)
{
    
    while(*data!='\0')	
    {
        TXREG = *data;          /* Load the character to be transmitted        */ 
        while(!TRMT);         /* Wait here till transmission is complete     */
        data++;
    }
     
}

void lcd_init(void)
{
    lcd_cmd(0x38);                 //2 line, 8 bit, 5x7 dots
    delay(30);
    lcd_cmd(0x0F);                 //display on, cursor blink
    delay(30);
    lcd_cmd(0x01);                 //clear lcd
    delay(30);
    lcd_cmd(0x06);                 //Entry mode, auto increment with no shift
    delay(30);
    
}

void lcd_cmd(char cmd)
{
    rs=0;
    PORTD= cmd;
    strobe();
}

void lcd_data(char data)
{
    rs=1;
    PORTD= data;
    strobe();
}

void strobe(void)
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

void send2lcd(char *lcd_string)
{
    while (*lcd_string != '\0')
    {
        lcd_data(*lcd_string);
        lcd_string++;
        delay(20);
    }
}
