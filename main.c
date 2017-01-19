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


void I2C2_WriteChar (char Adr, char Dat)    { 
  I2C2->CR1         |= 0x0100;       // send START bit 
  while (!(I2C2->SR1 & 0x0001)) {};  // wait for START condition (SB=1)  
  I2C2->DR           = 0xd0;         // slave address   -> DR & write 
  while (!(I2C2->SR1 & 0x0002)) {};  // wait for ADDRESS sent (ADDR=1)  
  int Status2        = I2C2->SR2;    // read status to clear flag 
  I2C2->DR           = Adr;          // Address in chip -> DR & write 
  while (!(I2C2->SR1 & 0x0080)) {};  // wait for DR empty (TxE) 
  I2C2->DR           = Dat;          // Dat -> DR & write 
  while (!(I2C2->SR1 & 0x0080)) {};  // wait for DR empty (TxE)  
  while (!(I2C2->SR1 & 0x0004)) {};  // wait for Byte sent (BTF) 
  I2C2->CR1         |= 0x0200;       // send STOP bit 
} 

void I2C_init(void) {
	
	  ///* Initialize USART2 at 9600 baud, TX: PB6, RX: PB7 */
  
RCC->AHB1ENR  |= 0x02;  // Enable clock for GPIOB
GPIOB->AFR[0] |= 0x77000000;  // select AF7 (UART1) for PB6, PB7 
GPIOB->MODER  |= 0x0a000;  // PB6, PB7 => AF mode
GPIOB->PUPDR  |= 0x05000;
	
  // initialize I2C block 
  RCC->APB1ENR  |= 0x00400000;       // Enable clock for I2C2 
  I2C2->CR2     |= 0x0008;           // clock == 8MHz!    
  I2C2->CCR     |= 0x0040;           // clock control register (270kHz) 
  I2C2->TRISE   |= 0x0009;           // rise time register 
  I2C2->CR1     |= 0x0001;           // I2C2 enable 
	
};

char I2C2_ReadChar (char Adr)    {   // procedure: RM0090, pg. 584! 
  I2C2->CR1         |= 0x0100;       // send START bit 
  while (!(I2C2->SR1 & 0x0001)) {};  // wait for START condition (SB=1)  
  I2C2->DR           = 0xd0;         // slave address -> DR      (LSB=1) 
  while (!(I2C2->SR1 & 0x0002)) {};  // wait for ADDRESS sent    (ADDR=1)  
  int Status2        = I2C2->SR2;    // read SR2 to clear flag 
  I2C2->DR           = Adr;          // register in chip -> DR  
  while (!(I2C2->SR1 & 0x0080)) {};  // wait for DR empty        (TxE=1) 
  while (!(I2C2->SR1 & 0x0004)) {};  // wait for ByteTransferred (BTF=1)  
  I2C2->CR1         |= 0x0100;       // send START bit 
  while (!(I2C2->SR1 & 0x0001)) {};  // wait for START condition (SB=1) 
  I2C2->DR           = 0xd1;         // slave address -> DR      (LSB=0) 
  while (!(I2C2->SR1 & 0x0002)) {};  // wait for ADDRESS sent    (ADDR=1) 
  int Status4        = I2C2->SR2;    // read status to clear flag 
  while (!(I2C2->SR1 & 0x0040)) {};  // wait for ByteReceived    (RxNE=1) 
  I2C2->CR1         |= 0x0200;       // send STOP bit 
  return ((char)I2C2->DR);           // return byte 
} 




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




