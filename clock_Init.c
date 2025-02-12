/*
 * clock_Init.c
 *
 *  Created on: 29 de dez de 2024
 *      Author: Francisco
 */

#include <stdint.h>
#include <tm4c123gh6pm.h>

void Clock_Init(long SYSDIV_var){

  // a) configure the system to use RCC2 for advanced features
  SYSCTL_RCC2_R |= 0x80000000;

  // b) bypass PLL e SYSDIV while initializing
  SYSCTL_RCC2_R |= 0x00000800;
    SYSCTL_RCC_R &= ~0x00400000;

  // c) select the crystal value and oscillator source
  SYSCTL_RCC_R &= ~0x000007C0;
  SYSCTL_RCC_R += 0x00000540;
  SYSCTL_RCC2_R &= ~0x00000070;
  SYSCTL_RCC2_R += 0x00000000;

  // d) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~0x00002000;

  // e) set the desired system divider and the system divider least significant bit
  SYSCTL_RCC2_R |= 0x40000000;
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000)
                  + (SYSDIV_var<<22);
    SYSCTL_RCC_R |= 0x00400000;

  // f) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&0x00000040)==0){};

  // g) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~0x00000800;
}

