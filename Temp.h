#ifndef TEMP_H_INCLUDED
#define TEMP_H_INCLUDED
//unsigned int readState;
//unsigned int dayIndex;
void initTemp(void);
void startMeasureTemp(void);
double measureTemp();
void TC0_Handler(void);
void startRecording();
void takeTemperatureRead(int months, int days, int hours, int minutes);
void showTempInfo();
struct LinkedList * findMin(struct LinkedList *first);
struct LinkedList * findMax(struct LinkedList *first);
double findAvg(struct LinkedList *first);
void checkAlarm();
void Set_Alarm(unsigned int state);
void showTempCursor(unsigned int cursor);
void setTempValues(unsigned int cursor, unsigned int value);
unsigned int checkReadState();
void dayIndexInc();
#endif