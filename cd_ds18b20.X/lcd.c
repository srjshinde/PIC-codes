#include <xc.h>
#include "lcd.h"

#define LCDPORT PORTD
#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define E PORTCbits.RC2



int  status=0;
#define lcd_delay 300

#define LCDMaxLines 2
#define LCDMaxChars 16
#define LineOne 0x80
#define LineTwo 0xc0

#define BlankSpace ' '


void delay(unsigned int j)
{
	unsigned int i=0;
	for(i=0;i<j;i++);
}

void lcd_init_write(unsigned char a)
{
	RS=0;
	RW=0;
	LCDPORT=a;
	E=1;
	delay(lcd_delay);
	E=0;
}


void lcd_com(unsigned char a)
{
	unsigned char temp;
	if(status)
	{
		status=0;
		goto next;
	}
	RS=0;
	next:
	RW=0;					
	temp=a;					
	temp&=0xf0;				// Mask Lower 4 Bits
	LCDPORT&=0x0f;			// Make No Affect on 0ther Port Pins
	LCDPORT|=temp;			// Send Higher Nibble to LCDPORT
	E=1;
	delay(lcd_delay);		//Send Enable Signal to LCD
	E=0;				
	temp=a<<4;				//Left Shift Byte Four Times
	temp&=0xf0;				// Mask Higher 4 Bits
	LCDPORT&=0x0f;         	// Make No Affect on 0ther Port Pins
	LCDPORT|=temp;			// Send Lower Nibble to LCDPORT
	E=1;
	delay(lcd_delay);		// Send Enable Signal to LCD
	E=0;
}

void lcd_data(unsigned char a)
{
	status=1;
	RS=1;
	lcd_com(a);
}
void lcd_init(void)
{
	delay(lcd_delay);
	//lcd_init_write(0x30);	//Special Sequence:Write Function Set.
	delay(lcd_delay);
	lcd_init_write(0x28);   // 0x20 for 4-bit
	delay(lcd_delay);
	lcd_com(0x0C);
	//lcd_com(0x28);         //Display Off, Cursor Off, Blink Off
	delay(lcd_delay);
	lcd_com(4);			   // Clear Screen & Returns the Cursor Home
	delay(lcd_delay);
	lcd_com(0x85);       
	delay(lcd_delay);       
	lcd_com(6);           //Inc cursor to the right when writing and don?t shift screen
	delay(lcd_delay);
	lcd_com(1);
	delay(lcd_delay);
}
void lcd_puts(char *str)
{
 	unsigned int i=0;
	for(;str[i]!=0;i++)
		lcd_data(str[i]);
}

void hex2lcd(unsigned char hex){
	char temp1,temp2;
	temp1 = hex;
	temp2=0;
	do{
		temp1 = temp1-100;
		if(temp1>=0)
			temp2++;
	} while(temp1>=0);
	if(temp2>0)
	lcd_data(temp2+0x30);
	temp2=0;
	temp1 = temp1+100;
	do{
		temp1 = temp1-10;
		if(temp1>=0)
			temp2++;
	} while(temp1>=0);
	lcd_data(temp2+0x30);
	temp2 = temp1+10;
	lcd_data(temp2+0x30);
}

void lcd_bcd(unsigned char uc_digit, unsigned int ui_number)
{
	//if(ui_number == 0){
	//	lcd_data('0');

	unsigned int ui_decimal[5] ={ 0 };	   
	//extract 5 single digit from ui_number
		ui_decimal[4] = ui_number/10000;	
		ui_decimal[3] = ui_number%10000;	
		ui_decimal[2] = ui_decimal[3]%1000;	
		ui_decimal[3] = ui_decimal[3]/1000;			
		ui_decimal[1] = ui_decimal[2]%100;			
		ui_decimal[2] = ui_decimal[2]/100;			
		ui_decimal[0] = ui_decimal[1]%10;			
		ui_decimal[1] = ui_decimal[1]/10;			
				

			if (ui_decimal[4]==0){
				if (ui_decimal[3]==0){
				    //lcd_data(ui_decimal[4] + 0x30);	//0
					//lcd_data(ui_decimal[3] + 0x30);	//1
					lcd_data(ui_decimal[2] + 0x30);	//0
					lcd_data('.');	
					lcd_data(ui_decimal[1] + 0x30);//1	
					lcd_data(ui_decimal[0] + 0x30);//2
				}
				else {
					//lcd_data(ui_decimal[4] + 0x30);	//0
					lcd_data(ui_decimal[3] + 0x30);	//1
					lcd_data(ui_decimal[2] + 0x30);	//0
					lcd_data('.');	
					lcd_data(ui_decimal[1] + 0x30);//1	
					lcd_data(ui_decimal[0] + 0x30);//2
				}
			}
			else {
				lcd_data(ui_decimal[4] + 0x30);	//0
				lcd_data(ui_decimal[3] + 0x30);	//1
				lcd_data(ui_decimal[2] + 0x30);	//0
				lcd_data('.');	
				lcd_data(ui_decimal[1] + 0x30);//1	
				lcd_data(ui_decimal[0] + 0x30);//2
			}
			

}

void LCD_ScrollMessage(char *msg_ptr)
{
  unsigned char i,j;

  for(i=0;msg_ptr[i];i++)        //Loop to display the string complete
    {                            //each time 16 chars are displayed and
                                 //pointer is incremented to point to next char

      lcd_com(LineOne);                   //Move the Cursor to first line

    for(j=0;j<LCDMaxChars && msg_ptr[i+j];j++)//loop to Display first 16 Chars
	  lcd_data(msg_ptr[i+j]);
	  delay(1000);                 //or till Null char

	for(j=j; j<LCDMaxChars; j++)               //If the chars are below 16
      lcd_data(BlankSpace);              //then display blank spaces
      delay(1000);
    }
}
