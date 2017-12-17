#include "stm32f4xx.h"    // the required include 
#include "lcd.h"
#include <stdio.h>
#include "main.h"
#include "usart2.h" 
#include "rng.h"
#include "delay.h" 
#include "system.h"
void slaveSelect_ctrl(_Bool state);
void SPI_send(uint8_t address, uint8_t data);
uint8_t SPI_read(uint8_t address);
char buffer2[20];
enum{ deselect=0, select=1 };
char buff1[20];
char buff2[20];
char buff3[20];

uint16_t MSBx, MSBy, MSBz,LSBx,LSBy,LSBz,Xg,Yg,Zg;



int main() { 


	SystemInit();

	SysTick_Config(168000000/1000);
	//USART1_init();
	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIODEN|RCC_AHB1ENR_GPIOEEN;;  // Enable clock for GPIOA and GPIOE
	

	GPIOA->MODER  |= GPIO_MODER_MODER5_1|GPIO_MODER_MODER7_1|GPIO_MODER_MODER6_1;
  GPIOB->MODER  |= GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0;  //  PA5,PA6, PA7 => AF mode
	GPIOD->MODER  |= GPIO_MODER_MODER12_0|GPIO_MODER_MODER13_0|GPIO_MODER_MODER14_0|GPIO_MODER_MODER15_0;
  GPIOE->MODER  |= GPIO_MODER_MODER3_0;
	GPIOA->AFR[0] |= (5<<20)|(5<<28)|(5<<24);  // select AF5 (SPI1) for PA5,PA6, PA7 
	RCC->APB2ENR  |= RCC_APB2ENR_SPI1EN;  // Enable clock for SPI1

  SPI1->CR1    |= SPI_CR1_BR_0|SPI_CR1_BR_1|SPI_CR1_BR_2|SPI_CR1_MSTR|SPI_CR1_SSM|SPI_CR1_SSI|SPI_CR1_CPOL|SPI_CR1_CPHA;
  SPI1->CR1 |= SPI_CR1_SPE;

LCD_init();



//NVIC_EnableIRQ(TIM7_IRQn);
	
	//RCC->APB1ENR  |= RCC_APB1ENR_TIM7EN;  // Enable clock for TIM7
  //TIM7->PSC = 2;
	//TIM7->ARR =1000;
	//TIM7->DIER |=0x01;
	
  //TIM7->CR1 |= 0x05;
	
  SPI_send(0x23, 0xc9);                         // resetting the accelerometer internal circuit
  SPI_send(0x20, 0x67);                         // 100Hz data update rate, block data update disable, x/y/z enabled 
  SPI_send(0x24, 0x80);                         // Anti aliasing filter bandwidth 800Hz, 16G (very sensitive), no self-test, 4-wire interface
  SPI_send(0x10, 0x00);                         // Output(X) = Measurement(X) - OFFSET(X) * 32;
  SPI_send(0x11, 0x00);                         // Output(Y) = Measurement(Y) - OFFSET(Y) * 32;
  SPI_send(0x12, 0x00);                         // Output(Z) = Measurement(Z) - OFFSET(Z) * 32;
	

	
while (1) {
MSBx = SPI_read(0x29);      // X-axis MSB
LSBx = SPI_read(0x28);     // X-axis LSB
Xg = (MSBx << 8) | (LSBx);  // Merging
	
MSBy = SPI_read(0x2b);      // X-axis MSB
LSBy = SPI_read(0x2a);     // X-axis LSB
Yg = (MSBy << 8) | (LSBy);  // Merging
	
MSBz = SPI_read(0x2d);      // X-axis MSB
LSBz = SPI_read(0x2c);     // X-axis LSB
Zg = (MSBz << 8) | (LSBz);  // Merging
	sprintf(buff1, "%d",Xg);
	sprintf(buff2, "%d",Yg);
	sprintf(buff3, "%d",Zg);
LCD_Text(1,buff1);
LCD_Text(2,buff2);
LCD_Text(3,buff3);

};
	
	};

void TIM7_IRQHandler(void)
{ if(TIM7->SR & TIM_SR_UIF) // if UIF flag is set
{ TIM7->SR &= ~TIM_SR_UIF; // clear UIF flag

sprintf(buffer2, "%u\n", RNG2_Get());
USART1_send(buffer2);


	
};
};




void USART1_IRQHandler(void)
{ if ( USART1->SR & USART_SR_RXNE){
uint8_t tmp;
	USART1->SR &= ~USART_SR_RXNE;

	tmp=USART1->DR;
	
	USART1->DR=tmp;
	
};
};














void SPI_send(uint8_t address, uint8_t data)
{
  
 	uint16_t 	rec;
slaveSelect_ctrl(select);

	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = address;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	rec = SPI1->DR;

 	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = data;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	rec = SPI1->DR;

 
  slaveSelect_ctrl(deselect);
}



uint8_t SPI_read(uint8_t address)
{
	uint16_t 	rec;
  slaveSelect_ctrl(select);
  address = 0x80 | address;                         // 0b10 - reading and clearing status
  
	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = address;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	rec = SPI1->DR;
 
	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = 0x0;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	rec = SPI1->DR;
 
  slaveSelect_ctrl(deselect);
 
  return rec;
}


void slaveSelect_ctrl(_Bool state){
	// PA2 CS
	
if(state){
GPIOE->BSRRH |= GPIO_BSRR_BS_3;
}
else{
while(SPI1->SR& SPI_SR_BSY);
GPIOE->BSRRL |= GPIO_BSRR_BS_3;
}
};





//RCC->AHB2ENR |=  RCC_AHB2ENR_RNGEN;

//RNG->CR |= RNG_CR_RNGEN;
