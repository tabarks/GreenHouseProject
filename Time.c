#include "Time.h"
#include "Base.h"
#include "DISP.h"
#include "Temp.h"
#include "Light.h"

unsigned int cnt = 0;
unsigned int cnt72 = 0;
unsigned int seconds = 0;
unsigned int minutes = 0;
unsigned int hours = 0;
unsigned int days = 0;
unsigned int months = 0;
unsigned int years = 0;
unsigned int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void SysTick_Handler()
{
	cnt++; // Every 0.05 second
	
	
	if (cnt%10==0) // one second
	{
          
	
		cnt = 0;
		seconds++;

                
                
		if (seconds==60) // one minute check
		{
			seconds = 0;
			minutes++;
                        
			if (minutes==60) // one hour check
			{
				minutes = 0;
				hours++;
				if (hours==24) // one day check
				{
					hours = 0;
					days++;
					//dayIndexInc(); ////////////////////////////////////////////////
					// one month check
					if (months==2) // check if it is the second month
					{
						if ((years%400==0)||((years%4==0)&&(years%100!=0))) // Leap Year !!
						{
							if (days>monthDays[months-1]+1) // 29 days not 28 !!
							{
								days = 1;
								months++;
							}
						}
					}
					else if (days>monthDays[months-1])
					{
						days = 1;
						months++;
						if (months>12)
						{
							months = 1;
							years++;
						}
					}
				}
			}
                        
                        // check if i have to record temp
			//if (checkReadState()==1)
			//{
			//	takeTemperatureRead(months, days,hours, minutes);
			//}
                        
		}
		checkAlarm();
		lightControl(hours);
	}
        
        // High speed simulation to simplify testing.
        if(cnt%10==0){
          if (checkReadState()==1)
          {
                cnt72++;
		takeTemperatureRead(months, days, hours, minutes);
                  if(cnt72==72)
                  {
                    cnt72 = 0;
                    dayIndexInc();
                  }
          }
        }
        
}

void showTime(void)
{
	// printing Days
	if (days<10)
	{
		printINT(0, 0, 0);
		printINT(days, 0, 1);
	}
	else
	{
		printINT(days, 0, 0);
	}
	printString("/", 1, 0, 2);
	// printing Months
	if (months<10)
	{
		printINT(0, 0, 3);
		printINT(months, 0, 4);
	}
	else
	{
		printINT(months, 0, 3);
	}
	printString("/", 1, 0, 5);
	// printing Years
	printINT(years, 0, 6);
	// printing hours
	if (hours<10)
	{
		printINT(0, 0, 22);
		printINT(hours, 0, 23);
	}
	else
	{
		printINT(hours, 0, 22);
	}
	printString(":", 1, 0, 24);
	// printing minutes
	if (minutes<10)
	{
		printINT(0, 0, 25);
		printINT(minutes, 0, 26);
	}
	else
	{
		printINT(minutes, 0, 25);
	}
	printString(":", 1, 0, 27);
	// printing seconds
	if (seconds<10)
	{
		printINT(0, 0, 28);
		printINT(seconds, 0, 29);
	}
	else
	{
		printINT(seconds, 0, 28);
	}
}

void setTimeValues(unsigned int cursor, unsigned int value)
{
	if(cursor==1)
	{
		days -= (days%100) - (days%10);
		days += value * 10;
	}
	else if(cursor==2)
	{
		days -= (days%10);
		days += value;
	}
	else if(cursor==3)
	{
		months -= (months%100) - (months%10);
		months += value * 10;
	}
	else if(cursor==4)
	{
		months -= (months%10);
		months += value;
	}
	else if(cursor==5)
	{
		years -= (years%10000) - (years%1000);
		years += value * 1000;
	}
	else if(cursor==6)
	{
		years -= (years%1000) - (years%100);
		years += value * 100;
	}
	else if(cursor==7)
	{
		years -= (years%100) - (years%10);
		years += value * 10;
	}
	else if(cursor==8)
	{
		years -= (years%10);
		years += value;
	}
	else if(cursor==9)
	{
		hours -= (hours%100) - (hours%10);
		hours += value * 10;
	}
	else if(cursor==10)
	{
		hours -= (hours%10);
		hours += value;
	}
	else if(cursor==11)
	{
		minutes -= (minutes%100) - (minutes%10);
		minutes += value * 10;
	}
	else if(cursor==12)
	{
		minutes -= (minutes%10);
		minutes += value;
	}
	else if(cursor==13)
	{
		seconds -= (seconds%100) - (seconds%10);
		seconds += value * 10;
	}
	else if(cursor==14)
	{
		seconds -= (seconds%10);
		seconds += value;
	}
}

void showTimeCursor(unsigned int cursor)
{
	if(cursor==1)
	setAddress(0, 0);
	else if(cursor==2)
	setAddress(0, 1);
	else if(cursor==3)
	setAddress(0, 3);
	else if(cursor==4)
	setAddress(0, 4);
	else if(cursor==5)
	setAddress(0, 6);
	else if(cursor==6)
	setAddress(0, 7);
	else if(cursor==7)
	setAddress(0, 8);
	else if(cursor==8)
	setAddress(0, 9);
	else if(cursor==9)
	setAddress(0, 22);
	else if(cursor==10)
	setAddress(0, 23);
	else if(cursor==11)
	setAddress(0, 25);
	else if(cursor==12)
	setAddress(0, 26);
	else if(cursor==13)
	setAddress(0, 28);
	else if(cursor==14)
	setAddress(0, 29);
	LCDprint('_');
}
