#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED
void SysTick_Handler();
void showTime(void);
void setTimeValues(unsigned int cursor, unsigned int value);
void showTimeCursor(unsigned int cursor);
#endif