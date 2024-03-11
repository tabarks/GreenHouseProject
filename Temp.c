#include <math.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "Temp.h"
#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Base.h"
#include "DISP.h"
unsigned int read = 0;
unsigned int tempFlag = 0;
unsigned int RA, RB;

unsigned int readState = 0; // start recording valus or not
unsigned int dayIndex = 0;
struct LinkedList *daysTemps[7];
int daysRecorded[7];
int monthsRecorded[7];

double MaxTemp = 0;
double MinTemp = 0;

unsigned int printVal;

unsigned int dayState[7] = {1, 0, 0, 0, 0, 0, 0};
struct LinkedList dayMin[7];
double dayAvg[7];
struct LinkedList dayMax[7];

unsigned int checkReadState()
{
	return readState;
}

void dayIndexInc()
{
  dayMin[dayIndex] = *findMin(daysTemps[dayIndex]); // calculating the values and save them in arrays
  dayAvg[dayIndex] = findAvg(daysTemps[dayIndex]);  // to save them, because i will free up the array.
  dayMax[dayIndex] = *findMax(daysTemps[dayIndex]);
  dayState[dayIndex] = 2;
    // 0: i didn't calculate the temp values
    // 1: I'm starting calculating the temp values
    // 2: I have finished reading the values.
  FreeLinkedList(&daysTemps[dayIndex]);  // Free up the array
  dayIndex++;
  if (dayIndex>=7)
  {
    dayIndex = 0;
  }
  dayState[dayIndex] = 1;
  daysTemps[dayIndex] = NULL;
}

void initTemp(void) {
	
	
	// Select Timer_Clock1 as TCCLK
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR)&(~(7));
	
	//Enable counter and make a sw_reset in TC0_CCR0
	*AT91C_TC0_CCR = 5<<0;
	
	
	// Load counter to A when TIOA falling in (TC0_CMR0)
	// Write 2 to LDRA on CMR (bits 17 and 16)
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR)|(1<<17); // set the bit 17
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR)&(~(1<<16)); // clear the bit 16
	
	
	// Load counter to B when TIOA rising in (TC0_CMR0)
	// Write 1 to LDRB on CMR (bits 19 and 18)
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR)|(1<<18); // set the bit 18
	*AT91C_TC0_CMR = (*AT91C_TC0_CMR)&(~(1<<19)); // clear the bit 19
	
	// Enable the interrupt (NVIC) with the inline declared function
	NVIC_ClearPendingIRQ(TC0_IRQn);
	NVIC_EnableIRQ(TC0_IRQn);  // TC0_IRQn
	
}
void startMeasureTemp(void)
{
	
	//Create a startpuls with a Delay(25);
	*AT91C_PIOB_OER = 1<<25;
	delay(25);
	*AT91C_PIOB_ODR = 1<<25;
	
	delay(60000);
	
	// make a sw_reset in TC0_CCR0
	*AT91C_TC0_CCR = 1<<2;
	
	// Clear old interrupt by read status reg
	*AT91C_TC0_SR;
	
	tempFlag = 0;
	
	//Enable interrupt for LDRBS
	*AT91C_TC0_IER = 1<<6;
	
}
void TC0_Handler(void) {
	*AT91C_TC0_IDR = (1<<6);
	tempFlag = 1;
}

double measureTemp()
{
	RA = *AT91C_TC0_RA;
	RB = *AT91C_TC0_RB;
	double temp = RB - RA;
	if(temp<200.0)
	return NAN;
	temp /= 5.0;
	// Timer_CLK1 = 42 MHz
	//TC_1 is 1/2 of masterClock, (masterClock = 84 MHz)
	temp /= 42.0;
	temp -= 273.15; // To C
	return temp;
}
void startRecording(){
	for (int i = 0; i < 7; i++)
	{
		FreeLinkedList(&daysTemps[i]); //// //////////////// Added
		daysTemps[i] = NULL;
	}
	dayIndex = 0;
	readState = 1;
}
void takeTemperatureRead(int months, int days, int hours, int minutes)
{
	daysRecorded[dayIndex] = days;
	monthsRecorded[dayIndex] = months;
	startMeasureTemp();
	//delay(100);
	double temp = measureTemp();
	while(isnan(temp))
	{
		startMeasureTemp();
		//delay(100);
		temp = measureTemp();
	}
	struct LinkedList *newN = newNode(60*hours+minutes, temp);
	if(newN==NULL)
	{
		startRecording();
		return;
	}
	insertFirst(&daysTemps[dayIndex], newN);
}

void showTempInfo()
{
  printString("date:   min:    avg:    max:", 28, 1, 0);
  for (int i = 0; i < 7; i++)
  {
    struct LinkedList minNode;
    double avgVal;
    struct LinkedList maxNode;
    if (dayState[i]==1)
    {
      if(daysTemps[i]==NULL){
        continue;
      }
      minNode = *findMin(daysTemps[i]);
      avgVal = findAvg(daysTemps[i]);
      maxNode = *findMax(daysTemps[i]);
    } 
    else if(dayState[i]==2)
    {
      minNode = dayMin[i];
      avgVal = dayAvg[i];
      maxNode = dayMax[i];
    }
    if (dayState[i]!=0)
    {
      printString("-",1,2+2*i,1);
      printINT(i+1,2+2*i,2);
      printString("-",1,2+2*i,3);
      if (daysRecorded[i]<10)
      {
        printString("0",1,3+(2*i),0);
        printINT(daysRecorded[i],3+(2*i),1);
      }
      else
      {
        printINT(daysRecorded[i],3+(2*i),0);
      }
      printString("/",1,3+2*i,2);
      if (monthsRecorded[i]<10)
      {
        printString("0",1,3+(2*i),3);
        printINT(monthsRecorded[i],3+(2*i),4);
      }
      else
      {
        printINT(monthsRecorded[i],3+(2*i),3);
      }
      unsigned int h = (minNode.minutes)/60;
      unsigned int m = (minNode.minutes)%60;
      printVal = maxNode.minutes;
      if (h<10)
      {
        printString("0",1,2+2*i, 8);
        printINT(h,2+2*i, 9);
      }
      else
      {
        printINT(h,2+2*i, 8);
      }
      printString(":",1,2+2*i,10);
      if (m<10)
      {
        printString("0",1,2+2*i, 11);
        printINT(m,2+2*i, 12);
      }
      else
      {
        printINT(m,2+2*i, 11);
      }
      printDouble(minNode.sensorData,2,3+2*i, 8);
        
        
      printDouble(avgVal,2,3+2*i, 16);

      h = (maxNode.minutes)/60;
      m = (maxNode.minutes)%60;
      if (h<10)
      {
        printString("0",1,2+2*i, 24);
        printINT(h,2+2*i, 25);
      }
      else
      {
        printINT(h,2+2*i, 24);
      }
      printString(":",1,2+2*i,26);
      if (m<10)
      {
        printString("0",1,2+2*i, 27);
        printINT(m,2+2*i, 28);
      }
      else
      {
        printINT(m,2+2*i, 27);
      }
      printDouble(maxNode.sensorData,2,3+2*i,24);
      printString(" ",1,2+2*i,29);
      printString(" ",1,3+2*i,29);
    }
  }
}

struct LinkedList * findMin(struct LinkedList *first)
{
	struct LinkedList *temp = first;
	if (temp==NULL)
	{
		return NULL;
	}
	struct LinkedList *mn = temp;
	while(temp!=NULL)
	{
		if(temp->sensorData < mn->sensorData)
		mn = temp;
		temp = temp->next;
	}
	return mn;
}

struct LinkedList * findMax(struct LinkedList *first)
{
	struct LinkedList *temp = first;
	if (temp==NULL)
	{
		return NULL;
	}
	struct LinkedList *mx = temp;
	while(temp!=NULL)
	{
		if(temp->sensorData > mx->sensorData)
		mx = temp;
		temp = temp->next;
	}
	return mx;
}

double findAvg(struct LinkedList *first)
{
	struct LinkedList *temp = first;
	if (temp==NULL)
	{
		return 0.0;
	}
	double sum = 0;
	double n = 0;
	while(temp!=NULL)
	{
		n++;
		sum += temp->sensorData;
		temp = temp->next;
	}
	return (sum/n);
}

void checkAlarm()
{
	startMeasureTemp();
	delay(100000); // X
	double temp = measureTemp();
	while(isnan(temp))
	{
		startMeasureTemp();
		delay(100000); // X
		temp = measureTemp();
	}
	if (temp<MinTemp||temp>MaxTemp)
	{
		Set_Alarm(1);
	}
	else
	{
		Set_Alarm(0);
	}
}
void Set_Alarm(unsigned int state)
{
	if (state==0)
	{
		*AT91C_PIOD_CODR = (1<<3);       // (PIOD) Clear Output Data Register
	}
	else
	{
		*AT91C_PIOD_SODR = (1<<3);       // (PIOD) Set Output Data Register
	}
}
void showTempCursor(unsigned int cursor)
{
	if(MinTemp < 10){
		printString("0",1,4,12);
		printDouble(MinTemp,2, 4, 13);
		}else{
		printDouble(MinTemp,2, 4, 12);
	}
	if(MaxTemp < 10){
		printString("0",1,6,12);
		printDouble(MaxTemp,2, 6, 13);
		}else{
		printDouble(MaxTemp,2, 6, 12);
	}
	if(cursor==1)
	setAddress(4, 12);
	else if(cursor==2)
	setAddress(4, 13);
	else if(cursor==3)
	setAddress(4, 15);
	else if(cursor==4)
	setAddress(4, 16);
	else if(cursor==5)
	setAddress(6, 12);
	else if(cursor==6)
	setAddress(6, 13);
	else if(cursor==7)
	setAddress(6, 15);
	else if(cursor==8)
	setAddress(6, 16);
	LCDprint('_');
}

void setTempValues(unsigned int cursor, unsigned int value)
{
	int intMN = MinTemp*100;
	int intMX = MaxTemp*100;
	if(cursor==1)
	{
		intMN -= (intMN%10000) - (intMN%1000);
		intMN += value * 1000;
	}
	else if(cursor==2)
	{
		intMN -= (intMN%1000) - (intMN%100);
		intMN += value * 100;
	}
	else if(cursor==3)
	{
		intMN -= (intMN%100) - (intMN%10);
		intMN += value * 10;
	}
	else if(cursor==4)
	{
		intMN -= (intMN%10);
		intMN += value;
	}
	else if(cursor==5)
	{
		intMX -= (intMX%10000) - (intMX%1000);
		intMX += value * 1000;
	}
	else if(cursor==6)
	{
		intMX -= (intMX%1000) - (intMX%100);
		intMX += value * 100;
	}
	else if(cursor==7)
	{
		intMX -= (intMX%100) - (intMX%10);
		intMX += value * 10;
	}
	else if(cursor==8)
	{
		intMX -= (intMX%10);
		intMX += value;
	}
	MinTemp = intMN;
	MaxTemp = intMX;
	MinTemp /= 100;
	MaxTemp /= 100;
}