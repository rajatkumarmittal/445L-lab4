#include "switch.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

int buttonL=0;

void Switch_Init(void){  unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x11;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000F000F; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x11;  //     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0
  //GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
  //GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
  //GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event
  //GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
  //GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
  //NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  //NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
}

void GPIOPortF_Handler(void){
	if(GPIO_PORTF_RIS_R&0x01){  // SW2 touch
    GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
  }
  if(GPIO_PORTF_RIS_R&0x10){  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
  }
	buttonL = 1;
}
