#include "stm32f4xx.h"
#include <stdio.h>
#include "usart2.h" 
#include "rng.h"
#include <lcd.h>
#include "delay.h" 
#include "system.h"

int Result[1024], Ptr = 0;


int main() { 
	SystemInit();
	SysTick_Config(168000000/1000);
	
RCC->AHB1ENR|= RCC_AHB1ENR_GPIOAEN;
	
GPIOA->MODER |= GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4|GPIO_MODER_MODER5; // PA2, PA2, PA4, PA5 analog mode

RCC->APB1ENR |=RCC_APB1ENR_DACEN;
DAC->CR |= DAC_CR_EN1|DAC_CR_EN2;
RCC->APB2ENR |= RCC_APB2ENR_ADC1EN|RCC_APB2ENR_ADC2EN;
ADC->CCR |= ADC_CCR_MULTI_1|ADC_CCR_MULTI_2;
ADC1->CR2 |= ADC_CR2_EXTEN_0;
ADC1->CR2 |= ADC_CR2_EXTSEL_1|ADC_CR2_EXTSEL_2;
ADC1->CR1 |= ADC_CR1_EOCIE;
ADC1->SQR3 |= ADC_SQR3_SQ1_1;
ADC2->SQR3 |= ADC_SQR3_SQ1_1|ADC_SQR3_SQ1_0;


ADC1->CR2 |= ADC_CR2_ADON;
ADC2->CR2 |= ADC_CR2_ADON;

NVIC_EnableIRQ(ADC_IRQn);	


RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
TIM2->ARR =1000; 
TIM2->CR2 |= TIM_CR2_MMS_1;
TIM2->CR1 |= TIM_CR1_CEN;
LCD_init();
LCD_clear();




	

	
	
	
while (1) {

	
};
	
	};



void ADC_IRQHandler(void)
{ 
Ptr = (Ptr + 1) & 1023; // Increment pointer and limit its value
Result[Ptr] = ADC1->DR; // save ADC in circular buffer & clear EOC flag
a=Result[Ptr];
DAC->DHR12R1 = Result[Ptr]; // pass current buffer -> DAC
DAC->DHR12R2 = Result[(Ptr - 100) & 1023]; // pass past buffer -> DAC

};








