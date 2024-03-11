#include "Base.h"
#include "system_sam3x.h"
#include "at91sam3x8.h"
void Config()
{
	// ______________________ PMC ____________________________________________
	*AT91C_PMC_PCER = (1<<AT91C_ID_PIOB)|
	(1<<AT91C_ID_PIOC)|
	(1<<AT91C_ID_PIOD)|
	(1<<AT91C_ID_TC0);     // (PMC) Peripheral Clock Enable Register
	*AT91C_PMC_PCER1 = (1<<4)|(1<<5);
	
	/*
	Port D:
	RESET  D0
	Button  D1
	OE1    D2
	LED1    D3
	*/
	*AT91C_PIOD_PER = 15;              // (PIOD) PIO Enable Register
	*AT91C_PIOD_OER = (1<<2)|(1<<3)|1; // (PIOD) Output Enable Register
	*AT91C_PIOD_ODR = (1<<1);          // (PIOD) Output Disable Register
	*AT91C_PIOD_PPUDR = 15;            // (PIOD) Pull-up Disable Register
	
	/*
	Port C:
	LED2	    C1
	Row1 - DB0  C2
	Row2 - DB1  C3
	Row3 - DB2  C4
	Row4 - DB3  C5
		   DB4	C6
	Col1 - DB5  C7
	Col2 - DB6  C8
	Col3 - DB7  C9
	RV			C19
	FS			C18
	WR			C17
	RD			C16
	CE			C15
	C/D			C14
	OE2			C13
	DIR			C12
	*/
          /*
          1 = (1<<0)
          10 ~ 2 dec  = (1<<1)
            100 ~ 4 dec = (1<<2)
              (1<<0)|(1<<2)  = 101 ~ 5 dec
        */
	// (PIOC) PIO Enable Register
	*AT91C_PIOC_PER = (1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)
	|(1<<12)|(1<<13)|(1<<14)|(1<<15)|(1<<16)|(1<<17)|(1<<18)|(1<<19);
	// (PIOC) Output Disable Register
	*AT91C_PIOC_ODR = (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9);
	// (PIOC) Output Enable Register
	*AT91C_PIOC_OER = (1<<1)|(1<<12)|(1<<13)|(1<<14)|(1<<15)|(1<<16)|(1<<17)|(1<<18)|(1<<19);
	// (PIOC) Pull-up Disable Register
	*AT91C_PIOC_PPUDR = (1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)
	|(1<<12)|(1<<13)|(1<<14)|(1<<15)|(1<<16)|(1<<17)|(1<<18)|(1<<19);
	
	//Disable output (74chip) for ICs
	*AT91C_PIOC_SODR = (1<<12);
	*AT91C_PIOD_SODR = (1<<2);
	
	//_________________________________ Servo _______________________________________
	
	// Let peripheral control the pin REG_PIOB_PDR
	*AT91C_PIOB_PDR = (1<<17);
	
	// Activate peripheral B to control the pin in REG_PIOB_ABSR
	*AT91C_PIOB_ABMR = (1<<17);
	*AT91C_PIOB_PPUDR = (1<<17);
	
	// Enable PWM-channel in ENA_CHID1
	*AT91C_PWMC_ENA = (1<<1);
	
	// Set Pre-scaler to Master_CLK/32 in PWM_CMRx
	*AT91C_PWMC_CH1_CMR = 5;
	
	// Write a value to PWM_CPRD (20ms)
	*AT91C_PWMC_CH1_CPRDR = 52500;
	
	/*
	Master_CLK = 84*10^6 Hz
	PWM_CLK = Master_CLK / 32 = 2625*10^3
	Period = 20*10^-3
	PWM_CPRD = Period * PWM_CLK = 2625*10^3 * 20*10^-3 = 52500
	*/
	
	//_________________________________ ADC _______________________________________
	*AT91C_ADCC_MR = (1<<9);
}
void delay(unsigned int value) {
	for (int i = 0; i < value; i++) {
		asm("nop");
	}
}

void delayMS(unsigned int value) {
	delay(value*16812);
}

int func(void){
	int Value = -1;
	
	//Clear OE KEY BUS (Active Low)
	*AT91C_PIOD_CODR = (1<<2);       // (PIOD) Set Output Data Register

	//Make all Column pin as output
	*AT91C_PIOC_OER = (7<<7);        // (PIOC) Output Enable Register
	
	//Make all Rows pin as input
	*AT91C_PIOC_ODR = (15<<2);        // (PIOC) Output Disable Register
	
	//Set all Column pin as high
	*AT91C_PIOC_SODR = (7<<7);       // (PIOC) Set Output Data Register
	
	//Loop Column
	for(int Col = 0; Col < 3; Col++)
	{
		//Clear one column at the time
		*AT91C_PIOC_CODR = (1<<(7+Col));
		
		////Loop Row
		for(int Row = 0; Row < 4; Row++)
		{
			//Read row and check if bit is zero
			if((*AT91C_PIOC_PDSR & (1 << (2+Row))) != (1 << (2+Row))) // (PIOC) If Pin Data Status Register = 0
			{
				//Value = Row*3+Col+1;
				Value = Row*3+Col+1;
			}
		}
		//end loop Row
		
		//Set the column again
		*AT91C_PIOC_SODR = (7<<7);       // (PIOC) Set Output Data Register
	}
	//End loop Column
	
	//Make all Column pin as input
	*AT91C_PIOC_ODR = (7<<7);        // (PIOC) Output Disable Register
	
	//Set OE KEY BUS
	*AT91C_PIOD_SODR = (1<<2);       // (PIOC) Set Output Data Register
	if(Value==11)
	Value = 0;
	//Return Value
	return Value;
}
