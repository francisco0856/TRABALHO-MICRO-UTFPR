#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <tm4c123gh6pm.h>
#include <UART.h>
#include <pwm.h>
#include <clock_Init.h>
#include <adc_Init.h>
#include <globalVariables.h>
#include <funcoes_gerais.h>


uint16_t periodpwm = 40000;
uint16_t dutypwm = 20000;


uint16_t hab_aquisicao=0; // Habilita a aquisição de dados (1 = habilitado, 0 = desabilitado)
uint32_t contstep=0;//
int16_t buffer_aqs[10000]; //
uint16_t indexbuf_aqs=0; // Variable to store the RPM of QEI1
int16_t cont_buff_aqs; // Habilita a aquisição de dados (1 = habilitado, 0 = desabilitado)
int16_t leitura_adc;

float aux=0;
float aux0=0;
float audio_normalizado=0; // Habilita a aquisição de dados (1 = habilitado, 0 = desabilitado)


void EnableInterrupts(void);
void Delay(uint32_t delay);

int main(void) {
    Clock_Init(4);
    UART_Init(); // Inicializa a UART
    adc_Init(3200); // Configuração do ADC para 25khz
    PWM3B_Init(periodpwm, dutypwm);
    PWM1_ENABLE_R |= 0x00000080; // Enable PF3/M1PWM7

    EnableInterrupts(); // Habilitar interrupções globais

    while (1) {
        // Transmite os dados do buffer via UART
            // Atualiza o PWM baseado nos dados do buffer
            audio_normalizado = (aux-2060)/(2150-2060);
            if (audio_normalizado < 0) audio_normalizado = 0;
            if (audio_normalizado >= 0.99) audio_normalizado = 0.98;
            PWM1_3_CMPA_R = periodpwm * audio_normalizado; // Duty entre 0 e 100%

    }
}

void ADC0Seq3_Handler(void) {

    if (ADC0_RIS_R & 0x08) {           // Verifica se a interrupção é do sequenciador 3

        leitura_adc = (int16_t) ADC0_SSFIFO3_R;
        aux = (float)leitura_adc;

    filtpbaixa(&aux, &aux0,  0.2008,  0.7992);

    if(hab_aquisicao == 1){
        contstep++;
        aquisicaobuffer(buffer_aqs, sizeof(buffer_aqs),&indexbuf_aqs,&cont_buff_aqs, 1, aux);
        if(contstep==25e3*0.4) //1 segundos em termos da interrupcao do ADC  2*10Khz por exemplo
        {
            hab_aquisicao=0;
            contstep=0;
            indexbuf_aqs=0;
        }
    }

    UART_OutUDec((int16_t)aux);
    UART_OutChar(';'); // Delimitador de dados

    }
    ADC0_ISC_R = 0x08;             // Limpa a interrupção do sequenciador 3

}



