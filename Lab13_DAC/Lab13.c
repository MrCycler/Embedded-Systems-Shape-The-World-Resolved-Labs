// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// March 13, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);
unsigned char x;
unsigned char previous;
int main(void){ // Real Lab13 
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
// PortE used for piano keys, PortB used for DAC        
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
	previous = 1;
  EnableInterrupts();  // enable after all initialization are done
  while(1){                
// input from keys to select tone
   x=Piano_In();
		if ((previous!=0)&&(x==0)){DisableInterrupts();  }
		if((x==1)&&(previous!=1)){ EnableInterrupts();Sound_Tone(9560);}//1}
		if((x==2)&&(previous!=2)){ EnableInterrupts();Sound_Tone(8518);}//6}
		if((x==4)&&(previous!=4)){ EnableInterrupts();Sound_Tone(7587);}//6}
		if((x==8)&&(previous!=8)){ EnableInterrupts();Sound_Tone(6378);}//4}
		previous = x; 
  }
            
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


