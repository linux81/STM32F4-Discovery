#include "stm32f4xx.h"    // the required include 
#include "LCD2x16_piotrek.h"
#include <stdio.h>
#include "main.h"
#include "usart2.h" 
#include "rng.h"



void Delay(__IO uint32_t time);
extern __IO uint32_t TimmingDelay;

char buffer2[20];

int main() { 
char buffer[20];

	SystemInit();
	SysTick_Config(SystemCoreClock/1000);

RCC->AHB2ENR |=  0x040;
RNG->CR |= RNG_CR_RNGEN;
	USART1_init();
	LCD_init();
	
	NVIC_EnableIRQ(TIM7_IRQn);
	
	RCC->APB1ENR  |= 0x020;  // Enable clock for TIM7
  TIM7->PSC = 2;
	TIM7->ARR =1000;
	TIM7->DIER |=0x01;
	
  TIM7->CR1 |= 0x05;
LCD_string(0,0,"RNG working!!!");
	
while (1) {


};
	
	

	

};







void Delay(__IO uint32_t time)
{
TimmingDelay = time;
while(TimmingDelay !=0);
};


__IO uint32_t TimmingDelay;
void SysTick_Handler(void)
{
if(TimmingDelay !=0)
{
TimmingDelay --;
}
};




void TIM7_IRQHandler(void)
{ if(TIM7->SR & TIM_SR_UIF) // if UIF flag is set
{ TIM7->SR &= ~TIM_SR_UIF; // clear UIF flag

sprintf(buffer2, "%u\n", RNG2_Get());
USART1_send(buffer2);

	
};
};




