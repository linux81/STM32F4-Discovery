#include "stm32f4xx.h"    // the required include 

#include <stdio.h>
#include "usart2.h" 
#include "rng.h"
#include <lcd.h>
#include "delay.h" 
#include "system.h"

//uint32_t table[1024];
void fill_tab(uint32_t *tab);
int Result[1024], Ptr = 0;
char buffer1[20];
char buffer2[20];
uint32_t a;

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
//ADC2->CR2 |= ADC_CR2_CONT;
	//ADC2->CR1 |= ADC_CR1_SCAN;

ADC1->CR2 |= ADC_CR2_ADON;
ADC2->CR2 |= ADC_CR2_ADON;

NVIC_EnableIRQ(ADC_IRQn);	
	//USART1_init();

RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
TIM2->ARR =1000;
TIM2->CR2 |= TIM_CR2_MMS_1;
TIM2->CR1 |= TIM_CR1_CEN;
LCD_init();
LCD_clear();
//RCC->AHB2ENR |=  RCC_AHB2ENR_RNGEN;

//RNG->CR |= RNG_CR_RNGEN;



	

	
	
	
while (1) {
//sprintf(buffer1, "%u", a);

//LCD_Text(1,buffer1);
	
};
	
	};



void ADC_IRQHandler(void)
{ // if UIF flag is set

//GPIOE->ODR |= 0x0100; // PE08 up
Ptr = (Ptr + 1) & 1023; // Increment pointer and limit its value
Result[Ptr] = ADC1->DR; // save ADC in circular buffer & clear EOC flag
a=Result[Ptr];
DAC->DHR12R1 = Result[Ptr]; // pass current buffer -> DAC
DAC->DHR12R2 = Result[(Ptr - 100) & 1023]; // pass past buffer -> DAC

//GPIOE->ODR &= ~0x0100; // PE08 down
;
};




void USART1_IRQHandler(void)
{ if ( USART1->SR & USART_SR_RXNE){
uint8_t tmp;
	USART1->SR &= ~USART_SR_RXNE;

	tmp=USART1->DR;
	
	USART1->DR=tmp;
	
};
};




/*

RCC->APB1ENR  |= RCC_APB1ENR_TIM7EN;  // Enable clock for TIM7
//TIM7->PSC = 1;
//TIM7->ARR =20000;
//TIM7->DIER |=0x01;
//TIM7->CR1 |= 0x05;



//NVIC_EnableIRQ(TIM7_IRQn);

void TIM7_IRQHandler(void)
{ if(TIM7->SR & TIM_SR_UIF) // if UIF flag is set
{ TIM7->SR &= ~TIM_SR_UIF; // clear UIF flag

sprintf(buffer2, "%u\n", RNG2_Get());
USART1_send(buffer2);
	i=i+1;

	
};
};

*/








void fill_tab(uint32_t *tab){
int i;
for(i=0;i<1024;i++){

tab[i]=RNG2_Get();
}

}





