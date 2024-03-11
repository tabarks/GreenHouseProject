#include "DISP.h"
#include "Base.h"
#include "system_sam3x.h"
#include "at91sam3x8.h"

void resetLCD(void){
	*AT91C_PIOD_CODR = 1;
	delay(100);
	*AT91C_PIOD_SODR = 1;
}

void Init_Display(void)
{
	//Set chip enable display
	*AT91C_PIOC_SODR = (1<<15);
	//Set write display
	*AT91C_PIOC_SODR = (1<<17);
	//Set read display
	*AT91C_PIOC_SODR = (1<<16);
	resetLCD();
	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x40);//Set text home address
	Write_Data_2_Display(0x00);
	Write_Data_2_Display(0x40);
	Write_Command_2_Display(0x42); //Set graphic home address
	Write_Data_2_Display(0x1e);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x41); // Set text area
	Write_Data_2_Display(0x1e);
	Write_Data_2_Display(0x00);
	Write_Command_2_Display(0x43); // Set graphic area
	Write_Command_2_Display(0x80); // text mode
	Write_Command_2_Display(0x94); // Text on graphic off
}

unsigned char Read_Status_Display(void){

	unsigned char Temp;
	//make databus as input - (PIOC) Output Disable Register
	*AT91C_PIOC_ODR = (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9);
	//Set dir as input (74chip, 1 = input)
	*AT91C_PIOC_SODR = (1<<13);
	//Clear/enable output (74chip 0 = enable)
	*AT91C_PIOC_CODR = (1<<12);
	//Set C/D
	*AT91C_PIOC_SODR = (1<<14);
	//Clear chip select display
	*AT91C_PIOC_CODR = (1<<15);
	//Clear read display
	*AT91C_PIOC_CODR = (1<<16);
	//Make a Delay
	delay(100);
	//Read data bus and save it in temp
	Temp = (*AT91C_PIOC_PDSR >> 2);
	//Set chip select display
	*AT91C_PIOC_SODR = (1<<15);
	//Set read display
	*AT91C_PIOC_SODR = (1<<16);
	//Disable output (74chip)
	*AT91C_PIOC_SODR = (1<<12);
	//Set dir as output (74chip)
	*AT91C_PIOC_CODR = (1<<13);
	//Return (Temp)
	return Temp;
}
void Write_Command_2_Display(unsigned char Command){
	//Wait until Read_Status_Display returns an OK
	while(Read_Status_Display() & 1 != 1)
	delay(2);
	//Clear databus
	*AT91C_PIOC_CODR = (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9);
	//Set Command to databus
	*AT91C_PIOC_SODR = (Command << 2);
	//Set dir as output (74chip)
	*AT91C_PIOC_CODR = (1<<13);
	//Enable output (74chip)
	*AT91C_PIOC_CODR = (1<<12);
	//Set databus as output
	*AT91C_PIOC_OER = (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9);
	//Set C/D signal High (1 = Command)
	*AT91C_PIOC_SODR = (1<<14);
	//Clear chip select display
	*AT91C_PIOC_CODR = (1<<15);
	//Clear write display
	*AT91C_PIOC_CODR = (1<<17);
	//Make a Delay
	delay(100);
	//Set chip enable display
	*AT91C_PIOC_SODR = (1<<15);
	//Set write display
	*AT91C_PIOC_SODR = (1<<17);
	//Disable output (74chip)
	*AT91C_PIOC_SODR = (1<<12);
	//Make databus as input
	*AT91C_PIOC_ODR = (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9);
}

void Write_Data_2_Display(unsigned char Data){
	//Wait until Read_Status_Display returns an OK
	while(Read_Status_Display() & 1 != 1)
	delay(2);
	//Clear databus
	*AT91C_PIOC_CODR = (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9);
	//Set Data to databus
	*AT91C_PIOC_SODR = (Data << 2);
	//Set dir as output (74chip)
	*AT91C_PIOC_CODR = (1<<13);
	//Enable output (74chip)
	*AT91C_PIOC_CODR = (1<<12);
	//Set databus as output
	*AT91C_PIOC_OER = (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9);
	//Clear C/D signal High (0 = Data)
	*AT91C_PIOC_CODR = (1<<14);
	//Clear chip select display
	*AT91C_PIOC_CODR = (1<<15);
	//Clear write display
	*AT91C_PIOC_CODR = (1<<17);
	//Make a Delay
	delay(100);
	//Set chip enable display
	*AT91C_PIOC_SODR = (1<<15);
	//Set write display
	*AT91C_PIOC_SODR = (1<<17);
	
	//Disable output (74chip)
	*AT91C_PIOC_SODR = (1<<12);
	//Make databus as input
	*AT91C_PIOC_ODR = (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9);
}

void setAddress(unsigned int row, unsigned int col)
{
	unsigned int p = 30*row+col; // calculate the address
	unsigned char p1 = (p & 255); // get the low part
	unsigned char p2 = (p >> 8); // get the high part
	Write_Data_2_Display(p1); // send the low part
	Write_Data_2_Display(p2); // send the high part
	// Set Address pointer
	Write_Command_2_Display(0x24); // send the command
}

void clearLCD(void)
{
	setAddress(0, 0);
	Write_Data_2_Display(0x00);
	for (int i = 0; i < 480; i++) {
		// Data Write and Increment ADP
		Write_Command_2_Display(0xC0);
	}
}

// Get char as ascii code
// Then convert it to LCD table
// After that send it to LCD
void LCDprint(unsigned char c) {
	if(c>='a'&&c<='z'){
		c = c - 'a' + 65;
	}
	else if(c>='A'&&c<='Z'){
		c = c - 'A' + 0x21;
	}
	else if(c>='0'&&c<='9'){
		c = c - '0' + 16;
	}
	else if(c == '.'){
		c = 14;
	}
	else if(c == ':'){
		c = 0x1A;
	}
	else if(c == ' '){
		c = 0;
	}
	else if(c == '/'){
		c = 0x0F;
	}
	else if(c == '_'){
		c = 0x3F;
	}
	else if(c == '-'){
		c = 0x0D;
	}
	Write_Data_2_Display(c);
	// Data Write and Non-Variable ADP
	Write_Command_2_Display(0xC0);
}

void printINT(int n, unsigned int row, unsigned int col){
	setAddress(row, col);
	if(n==0){
		LCDprint('0');
		return;
	}
	int s[10];
	for(int i = 0; i < 10; i++)
	s[i] = -1;
	int i = 0;
	while(n){
		int d = n%10;
		s[i] = d;
		n /= 10;
		i++;
	}
	for(int j = 9; j >= 0; j--){
		if(s[j]!=-1)
		{
			LCDprint(s[j]+'0');
		}
	}
}

void printDouble(double n, int d, unsigned int row, unsigned int col){
	setAddress(row, col);
	int s[10];
	for(int i = 0; i < 10; i++)
	s[i] = -1;
	int i = 0;
	int nint = n;
	if(n<1.0)
	{
		LCDprint('0');
	}
	n -= nint;
	while(nint){
		int d = nint%10;
		s[i] = d;
		nint /= 10;
		i++;
	}
	for(int j = 9; j >= 0; j--){
		if(s[j]!=-1)
		{
			LCDprint(s[j]+'0');
		}
	}
	LCDprint('.');
	for(int j = 0; j < d; j++)
	{
		n*=10.0;
		int d = n;
		n -= d;
		LCDprint(d+'0');
	}
}

void printString(char s[], int size, unsigned int row, unsigned int col)
{
	setAddress(row, col);
	for(int i = 0; i < size; i++){
		LCDprint(s[i]);
	}
}