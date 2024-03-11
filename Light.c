#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "Light.h"

double measureLight(void){
	// Select channel in ADC_CHER
	*AT91C_ADCC_CHER = (1<<2);
	
	// Start an ADC in ADC_CR
	*AT91C_ADCC_CR = (1<<1);
	while((*AT91C_ADCC_SR & (1<<24)) != (1<<24));
	
	// get the first 12 bits from the LCDR to read the analog value from 0 to 4095
	double ret = (*AT91C_ADCC_LCDR)&((1<<12)-1);
	
	// calculate the voltage by scaling
	// 12 bits
	// 3.3V -> 12 as ones
	// bits -> volt
	// x ~ ret
	// 3.3 ~ 4096
	// x = 3.3*ret/4096
	ret = (3.3*ret)/4096.0;
	
	return ret;
}
void setServo(int angle)
{
	*AT91C_PWMC_CH1_CDTYUPDR = 1800+angle*27;
}
void Set_Light(unsigned int st)
{
	if (st==0)
	{
		*AT91C_PIOC_CODR = (1<<1);       // (PIOC) Clear Output Data Register
	}
	else
	{
		*AT91C_PIOC_SODR = (1<<1);       // (PIOC) Set Output Data Register
	}
}
void lightControl(unsigned int hours)
{
	if (hours>=3&&hours<19) // if we are in the working time 3 am to 7 pm (16 hours)
	{
		setServo(0); // Uncovering the shading system
		if(measureLight()>1.5) // Checking the light state (more than 1.5 volt -> low light)
		{
			Set_Light(1); // If it is low then turn the led on
		}
		else
		{
			Set_Light(0); // Or turn the led off
		}
                
	}
	else
	{
		Set_Light(0); // Turn the led off
		setServo(90); // Covering the shading system
	}
}