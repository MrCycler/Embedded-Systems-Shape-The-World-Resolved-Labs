// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 11, 2014

// Lab 8
//      Jon Valvano and Ramesh Yerraballi
//      November 21, 2013

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****

void LED_Swicht_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x10;           // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
                                    // 2) no need to unlock PA2
  GPIO_PORTE_PCTL_R &= ~0x000000FF; // 3) regular GPIO for PE1
  GPIO_PORTE_AMSEL_R &= ~0x03;      // 4) disable analog function on PE1 and PE0
  GPIO_PORTE_DIR_R |= 0x02;         // 5) set direction to output PE1
	GPIO_PORTD_DIR_R &= ~0x01;        //input in PE0
  GPIO_PORTE_AFSEL_R &= ~0x03;      // 6) regular port function PE1,PE0
  GPIO_PORTE_DEN_R |= 0x03;         // 7) enable digital port PE1,PE0
}

 //Make PE1 high
void LED_On(void){
  GPIO_PORTE_DATA_R |= 0x02;
}
// Make PE1 low
void LED_Off(void){
  GPIO_PORTE_DATA_R &= ~0x02;
}
void Delay100ms(unsigned long);
// PE0, PB0, or PA2 connected to positive logic momentary switch using 10 k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ unsigned long in;
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1);  // activate grader and set system clock to 80 MHz
  LED_Swicht_Init();
	
  EnableInterrupts();           // enable interrupts for the grader
  LED_On();
	
	while(1){
		
		LED_On();
    in = (GPIO_PORTE_DATA_R&0x01); // in 0 if not pressed, 1 if pressed
    if (in == 1){ Delay100ms(1);  LED_Off(); Delay100ms(1);}
		
  }
  
}
void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms	
  }}