// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.  
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is 
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
// March 8, 2014

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
unsigned long H;
unsigned char status;
unsigned char astatus;

#include "TExaS.h"
#include "..//tm4c123gh6pm.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode
unsigned char x=0;
// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void){ 
  SYSCTL_RCGC2_R |= 0x00000001; // activate clock for port A
  H = 90908;                // 40Mhz
  GPIO_PORTA_AMSEL_R &= ~0x24;      // disable analog functionality on PA5
  GPIO_PORTA_PCTL_R &= ~0x00F00F00; // configure PA5 as GPIO
  GPIO_PORTA_DIR_R |= 0x24;     // make PA5 out
  GPIO_PORTA_DR8R_R |= 0x24;    // enable 8 mA drive on PA5
  GPIO_PORTA_AFSEL_R &= ~0x24;  // disable alt funct on PA5
  GPIO_PORTA_DEN_R |= 0x24;     // enable digital I/O on PA5
  GPIO_PORTA_DATA_R &= ~0x24;   // make PA5 low
  NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
  NVIC_ST_RELOAD_R = H;       // reload value for 500us
  NVIC_ST_CURRENT_R = 0;        // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
  NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts ,todavia no lo habilitaremos
  
	GPIO_PORTA_DIR_R &= ~0x08;    // (c) make PA3 in (built-in button)
  GPIO_PORTA_AFSEL_R &= ~0x08;  //     disable alt funct on PA3
  GPIO_PORTA_DEN_R |= 0x08;     //     enable digital I/O on PA3
  GPIO_PORTA_PCTL_R &= ~0x0000F000; //  configure PA3 as GPIO
  GPIO_PORTA_AMSEL_R &= ~0x08;  //    disable analog functionality on PA3
  GPIO_PORTA_PUR_R &= ~ 0x08;     //     enable weak pull-up on PA3
  GPIO_PORTA_IS_R &= ~0x08;     // (d) PA3 is edge-sensitive
  GPIO_PORTA_IBE_R &= ~0x08;    //     PA3 is not both edges
  GPIO_PORTA_IEV_R |=0x08;    //     PA3 rising edge event
  GPIO_PORTA_ICR_R = 0x08;      // (e) clear flag3
  GPIO_PORTA_IM_R |= 0x00;      // (f) arm interrupt on PA3
	
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFFFF0F)|0x00000020; // (g) priority 5
  NVIC_EN0_R = 0x00010000;      // (h) enable interrupt 30 in NVIC

}
/*
void GPIOPortA_Handler(void){ 
 GPIO_PORTA_ICR_R = 0x08;     
if(x==0){x=1;}else{x=0;}
    // acknowledge flag4
}
*/

// called at 880 Hz
void SysTick_Handler(void){status= GPIO_PORTA_DATA_R&0x08;
	if ((status==8)&&(astatus==0)){if(x==1){x=0;}else{x=1;}}
	astatus=status;

if(x==1){GPIO_PORTA_DATA_R ^= 0x04;}    // toggle PA2

}

int main(void){// activate grader and set system clock to 80 MHz
	
  TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); 
  Sound_Init();         
  EnableInterrupts();   // enable after all initialization are done
	GPIO_PORTA_DATA_R = 0x00;
  while(1){
    // main program is free to perform other tasks
    // do not use WaitForInterrupt() here, it may cause the TExaS to crash
  }
}
