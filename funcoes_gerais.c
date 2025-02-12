/*
 * funcoes_gerais.c
 *
 *  Created on: 6 de fev de 2025
 *      Author: Francisco
 */



#include <tm4c123gh6pm.h>

void filtpbaixa(float *x, float *x0, float K1, float K2)
{
    *x = *x *K1 + *x0 *K2;
    *x0 = *x;
}

void aquisicaobuffer(int16_t *buffer, int sizebuffer, uint16_t *indexbuffer, uint16_t *cont, uint16_t contmax, float variavel)
{
    *cont += 1;
    if(*cont==contmax)
    {
        if(*indexbuffer < sizebuffer)
        {
            buffer[*indexbuffer] =  (int16_t) (variavel);
            *indexbuffer +=1;
        }
    *cont=0;
    }
}
