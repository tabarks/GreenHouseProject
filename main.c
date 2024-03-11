#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Base.h"
#include "DISP.h"
#include "Time.h"
#include "Temp.h"
#include "Light.h"
#include "LinkedList.h"

void Set_Led(unsigned int nled);
void ReadButton(unsigned int *nButton);
int func(void);
void setServo(int angle);
double startMeasureLight(void);

unsigned int button = 0;
signed int status = 0;
unsigned int servoAngle = 0;
double light = 0;
double T = 0;

unsigned int Mode = 0;
unsigned int cursor = 0;


int main()
{
	SystemInit();
	Config();
	initTemp();
	Init_Display();
	clearLCD();
	SysTick_Config(SystemCoreClock*0.1);
        printString("Welcome..", 9, 7, 11);
        delayMS(2000);
        clearLCD();
	while(1)
	{
		showTime();
		if (Mode==0)
		{
			printString("1- Time Configuration", 21, 2, 1);
			printString("2- Start Measurements", 21, 3, 1);
			printString("3- Measurements Infos", 21, 4, 1);
			printString("4- Set Temp Limits", 18, 5, 1);
			status = func();
			if (status!=-1)
			{
				if (1<=status&&status<=4)
				{
					clearLCD();
					Mode = status;
					cursor = 1;
				}
				delayMS(500);
			}
		}
		else if (Mode==1)
		{
			showTimeCursor(cursor);
			printString("Time Configuration...", 21, 2, 1);
			status = func();
			if (status!=-1)
			{
				if (status==12){
					cursor++;
				}
				else if (status==10){
					if(cursor > 1)
					cursor--;
				}
				else
				{
					setTimeValues(cursor, status);
					if(cursor < 14)
					cursor++;
				}
				if (cursor>14)
				{
					clearLCD();
					Mode = 0;
					cursor = 1;
				}
				delayMS(500);
			}
		}
		else if (Mode==2)
		{
			printString("Start Measurements...", 21, 2, 1);
			startRecording();
                        delayMS(1000);
			clearLCD();
			Mode = 0;
		}
		else if (Mode==3)
		{
			showTempInfo();
			status = func();
			if (status!=-1)
			{
				if (status==12){
					clearLCD();
					Mode = 0;
				}
				delayMS(500);
			}
		}
		else if (Mode==4)
		{
			printString("Set Temp Limits...", 18, 2, 1);
			printString("Min Limit:", 10, 4, 1);
			printString("Max Limit:", 10, 6, 1);
			showTempCursor(cursor);
			status = func();
			if (status!=-1)
			{
				if (status==12){
					cursor++;
				}
				else if (status==10){
					if(cursor > 1)
					cursor--;
				}
				else
				{
					setTempValues(cursor, status);
					if(cursor < 8)
					cursor++;
				}
				if (cursor>8)
				{
					clearLCD();
					Mode = 0;
					cursor = 1;
				}
				delayMS(500);
			}
		}
		delayMS(100);
	}
	return 0;
}

void ReadButton(unsigned int *nButton)
{
	if (((*AT91C_PIOD_PDSR)  & (1 << 1)) == (1 << 1))   // (PIOD) If Pin Data Status Register = 1
	*nButton = 0;                                       // Button not pressed
	else
	*nButton = 1;                                       //Button pressed
}