/*
 * pwm.c
 *
 *  Created on: 22 de dez de 2024
 *      Author: Francisco
 */

#include <tm4c123gh6pm.h>
#include <pwm.h>
//PF3

void PWM3B_Init(uint16_t period, uint16_t duty) { ///verde
    SYSCTL_RCGCPWM_R |= 0x02;             // 1) activate PWM modulo 1
    SYSCTL_RCGCGPIO_R |= 0x20;            // 2) activate port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0) {}; // wait for port F ready

    GPIO_PORTF_AFSEL_R |= 0x08;           // 3)  M1 módulo 1 - Saida do modulo 1
    GPIO_PORTF_PCTL_R &= ~0x0000F000;    // 4)Limpando os bits do pino PF3
    GPIO_PORTF_PCTL_R |= 0x5000;      // 4) configure PF3 as M1PWM7 [0101 0000 0000 0000]
    GPIO_PORTF_AMSEL_R &= ~0x08;          // 5) disable analog functionality on PF 1[0000]
    GPIO_PORTF_DEN_R |= 0x08;             // 6) enable digital I/O on PF3 [1000]

    SYSCTL_RCC_R = 0x00100000 |           // 7) use PWM divider
                   (SYSCTL_RCC_R & (~0x000E0000)); // configure for /2 divider
    PWM1_3_CTL_R = 0;                     // 8) re-loading down-counting mode for generator 2
    PWM1_3_GENB_R = 0xC8;                 // 9) low on LOAD, high on CMPA down
    PWM1_3_LOAD_R = period - 1;           // 10) cycles needed to count down to 0
    PWM1_3_CMPA_R = duty - 1;             // 11) count value when output rises
    PWM1_3_CTL_R |= 0x00000001;           // 12) start PWM7 generator 2
    PWM1_2_CTL_R |= 0x00000001;           // 12) start PWM5 generator 2
}



