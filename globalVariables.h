/*!
 * @author      Yash Bansod
 * @date        29th September 2017
 *
 * @brief       Header containing the global variables used in the program
 * @file        golbalVariables.h
 */
/* -----------------------          Include Files       --------------------- */
#include <stdint.h>                         // Library of Standard Integer Types
#include <stdbool.h>                        // Library of Standard Boolean Types

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

/* -----------------------      Global Variables        --------------------- */
extern uint16_t hab_aquisicao; // Habilita a aquisição de dados (1 = habilitado, 0 = desabilitado)
extern uint32_t contstep; //variavel de contagem auxiliar para aquisicao de dados
extern int16_t buffer_aqs[10000]; //buffer para leitura do adc
extern uint16_t indexbuf_aqs; //index para funcao de aquisicao do buffer
extern int16_t cont_buff_aqs; //variavel auxiliar de contagem para a funcao de aquisicao
extern float audio_normalizado; //Normaliza a amplitude do sinal de audio entre 0 a 1
extern int16_t leitura_adc; //armazena o valor do adc
extern float aux;  //variavel para o filtro passa baixa
extern float aux0; //variavel anterior para o filtro passa baixa
extern uint16_t periodpwm;
extern uint16_t dutypwm;


#endif /* GLOBALVARIABLES_H_ */
