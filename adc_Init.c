/*
 * adc_Init.c
 *
 *  Created on: 29 de dez de 2024
 *      Author: Francisco
 */


#include <tm4c123gh6pm.h>

// ------------ adcinit ------------
// Inicializa o ADC0 para uso geral
void adc_Init (uint32_t period) {

    SYSCTL_RCGCADC_R |= 0x01;       // (1) Ativar ADC0
    SYSCTL_RCGCGPIO_R |= 0x10;     // Clock para porta E
    while ((SYSCTL_PRGPIO_R & 0x10) == 0) {}; // Aguarda o clock estabilizar

    GPIO_PORTE_DIR_R &= ~0x20;     // PE5 como entrada 0b100000
    GPIO_PORTE_AFSEL_R |= 0x20;    // Função alternativa em PE5
    GPIO_PORTE_DEN_R &= ~0x20;     // Desabilitar I/O digital em PE5
    GPIO_PORTE_AMSEL_R |= 0x20;    // Habilitar função analógica em PE5

    ADC0_PC_R = 0x01;              // (2) Configurar para 125K samples/segundo
    ADC0_SSPRI_R = 0x3210;         // (3) Prioridade: Seq 0 > Seq 1 > Seq 2 > Seq 3
    SYSCTL_RCGCTIMER_R |= 0x01;    // (4) Ativar Timer0
    while ((SYSCTL_RCGCGPIO_R & 0x10) == 0) {}; // Aguarda o clock estabilizar

    TIMER0_CTL_R = 0x00000000;     // Desabilitar Timer0 durante a configuração
    TIMER0_CTL_R |= 0x00000020;    // Habilitar Timer0A como gatilho para ADC
    TIMER0_CFG_R = 0;              // Configurar Timer como 32 bits
    TIMER0_TAMR_R = 0x00000002;    // Configurar para modo periódico
    TIMER0_TAPR_R = 0;             // Prescaler para o gatilho
    TIMER0_TAILR_R = period - 1;   // Iniciar valor do Timer para o gatilho
    TIMER0_IMR_R = 0x00000000;     // Desabilitar todas as interrupções
    TIMER0_CTL_R |= 0x00000001;    // Habilitar Timer0A

    ADC0_ACTSS_R &= ~0x08;         // (5) Desabilitar Sequenciador 3
    ADC0_EMUX_R = (ADC0_EMUX_R & 0xFFFF0FFF) + 0x5000; // (6) Gatilho baseado no Timer
    ADC0_SSMUX3_R = 8;             // PE5 é o canal analógico 8
    ADC0_SSCTL3_R = 0x06;          // (8) Configurar flag de fim e interrupção
    ADC0_IM_R |= 0x08;             // (9) Habilitar interrupções para Seq 3
    ADC0_ACTSS_R |= 0x08;          // (10) Habilitar Sequenciador 3
    NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFF00FF) | 0x00006000; // (11) Prioridade 3
    NVIC_EN0_R = 1 << 17;          // (12) Habilitar interrupção 17 no NVIC
}



