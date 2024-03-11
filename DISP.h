#ifndef DISP_H_INCLUDED
#define DISP_H_INCLUDED
void resetLCD(void);
void Init_Display(void);
unsigned char Read_Status_Display(void);
void Write_Command_2_Display(unsigned char Command);
void Write_Data_2_Display(unsigned char Data);
void setAddress(unsigned int row, unsigned int col);
void clearLCD(void);
void LCDprint(unsigned char c);
void printINT(int n, unsigned int row, unsigned int col);
void printDouble(double n, int d, unsigned int row, unsigned int col);
void printString(char s[], int size, unsigned int row, unsigned int coly);
void setFont(char font);
#endif