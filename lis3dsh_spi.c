#include "stm32f4xx.h"    // the required include 
#include "LCD2x16_piotrek.h"
#include <stdio.h>
#include "main.h"
#include "usart2.h" 
#include "rng.h"

void SPI1_init(void);
void slaveSelect_ctrl(_Bool state);
void Delay(__IO uint32_t time);
extern __IO uint32_t TimmingDelay;
char buffer[20];
char buffer2[20];

void SPI_send(uint8_t address, uint8_t data);
uint8_t SPI_read(uint8_t address);
enum{ deselect=0, select=1 };
void Sort_Signed(int16_t A[], uint8_t L);       // Bubble sort min to max, input: Array/Length
float gToDegrees(float V, float H);             // output: degrees between two planes, input: Vertical/Horizontal force



uint8_t i;
uint8_t MSB, LSB,MSB1,LSB1,MSB2, LSB2;
int32_t Xg, Zg,Yg;                                 // 16-bit values from accelerometer
int16_t x_array[100];                           // 100 samples for X-axis
int16_t z_array[100];                           // 100 samples for Z-axis
float x_average;                                // x average of samples
float z_average;                                // z average of samples
float zx_theta;                                 // degrees between Z and X planes
char print_buffer[20];                          // printing the values in Putty














int main() { 


	SystemInit();
	SysTick_Config(SystemCoreClock/1000);
	USART1_init();
	LCD_init();
SPI1_init();
slaveSelect_ctrl(deselect);
	
	
	//SPI_send(0x23, 0xc9);                         // resetting the accelerometer internal circuit
  SPI_send(0x20, 0x67);                         // 100Hz data update rate, block data update disable, x/y/z enabled 
  SPI_send(0x24, 0x20);                         // Anti aliasing filter bandwidth 800Hz, 16G (very sensitive), no self-test, 4-wire interface
  SPI_send(0x10, 0x00);                         // Output(X) = Measurement(X) - OFFSET(X) * 32;
  SPI_send(0x11, 0x00);                         // Output(Y) = Measurement(Y) - OFFSET(Y) * 32;
  SPI_send(0x12, 0x00);                         // Output(Z) = Measurement(Z) - OFFSET(Z) * 32;
	
	
	
	
while (1) {

      MSB = SPI_read(0x29);                     // X-axis MSB
      LSB = SPI_read(0x28);                     // X-axis LSB
      Xg = (MSB << 8) | (LSB);                  // Merging
      MSB1 = SPI_read(0x2A);                     // Y-axis MSB
      LSB1 = SPI_read(0x2B);                     // Y-axis LSB
      Yg = (MSB1 << 8) | (LSB1);                  // Merging
   
      MSB2 = SPI_read(0x2d);                     // Z-axis MSB
      LSB2 = SPI_read(0x2c);                     // Z-axis LSB
      Zg = (MSB2 << 8) | (LSB2);                  // Merging


 
  sprintf(print_buffer, "x: %d y: %d\n",Xg,Yg);
	sprintf(buffer, "z: %d",Zg);
 
    LCD_string(0,0,print_buffer);
	LCD_string(0,1,buffer);

		USART1_send(print_buffer); 

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




/*void TIM7_IRQHandler(void)
{ if(TIM7->SR & TIM_SR_UIF) // if UIF flag is set
{ TIM7->SR &= ~TIM_SR_UIF; // clear UIF flag

sprintf(buffer2, "%u\n", RNG2_Get());
USART1_send(buffer2);
	i=i+1;

	
};
};*/

void SPI1_init(void){

	RCC->AHB1ENR  |= 0x11;  // Enable clock for GPIOA and GPIOE
	

	GPIOA->MODER  |= (1<<11)|(1<<13)|(1<<15);  //  PA5,PA6, PA7 => AF mode
	GPIOE->MODER  |= (1<<6);  //  PE3 => OUTPUT mode
	GPIOA->AFR[0] |= 0x55500000;  // select AF5 (SPI1) for PA5,PA6, PA7 
	RCC->APB2ENR  |= 1<<12;  // Enable clock for SPI1
  SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_CPHA | SPI_CR1_CPOL;      // SSM

	
  SPI1->CR1    |= 0x040;      // SPI1 ENABLE	
}



//RCC->AHB2ENR |=  0x040;
//RCC->AHB1ENR |=  1<<3;
//GPIOD->MODER|=1<<24|1<<26|1<<28|1<<30;
//RNG->CR |= RNG_CR_RNGEN;

	
	//NVIC_EnableIRQ(TIM7_IRQn);
	
	//RCC->APB1ENR  |= 0x020;  // Enable clock for TIM7
  //TIM7->PSC = 2;
	//TIM7->ARR =1000;
	//TIM7->DIER |=0x01;
	
  //TIM7->CR1 |= 0x05;
	//sprintf(buffer, "%u\n",i);
	//LCD_string(0,0,buffer);



void slaveSelect_ctrl(_Bool state){
if(state){
GPIOE->BSRRH |= 1<<3;
}
else{
while(SPI1->SR& SPI_SR_BSY);
GPIOE->BSRRL |= 1<<3;
}
};

void SPI_send(uint8_t address, uint8_t data)
{
  slaveSelect_ctrl(select);
 uint16_t Data;
	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = address;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	Data = SPI1->DR;
 
	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = data;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	Data = SPI1->DR;
 
  slaveSelect_ctrl(deselect);
}



uint8_t SPI_read(uint8_t address)
{
  slaveSelect_ctrl(select);
  address = 0x80 | address;                         // 0b10 - reading and clearing status
  uint16_t Data;
	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = address;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	Data = SPI1->DR;
 
	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = 0x00;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	Data = SPI1->DR;
 
 slaveSelect_ctrl(deselect);
 
  return Data;
}


void Sort_Signed(int16_t A[], uint8_t L)
{
  uint8_t i = 0;
  uint8_t status = 1;
 
  while(status == 1)
  {
    status = 0;
    for(i = 0; i < L-1; i++)
    {
      if (A[i] > A[i+1])
      {
        A[i]^=A[i+1];
        A[i+1]^=A[i];
        A[i]^=A[i+1];
        status = 1;    
      }
    }
  }
}



float gToDegrees(float V, float H)               // refer to the orientation pic above
{
  float retval;
  uint16_t orientation;
 
  if (H == 0) H = 0.001;                         // preventing division by zero
  if (V == 0) V = 0.001;                         // preventing division by zero
 
  if ((H > 0) && (V > 0)) orientation = 0;
  if ((H < 0) && (V > 0)) orientation = 90; 
  if ((H < 0) && (V < 0)) orientation = 180;
  if ((H > 0) && (V < 0)) orientation = 270;
 
  retval = ((atan(V/H)/3.14159)*180);
  if (retval < 0) retval += 90;
  retval = abs(retval) + orientation;
  return retval;
	
	
	
	/*
		    for(i = 0; i < 100; i++)                    // getting 100 samples
    {
      MSB = SPI_read(0x29);                     // X-axis MSB
      LSB = SPI_read(0x28);                     // X-axis LSB
      Xg = (MSB << 8) | (LSB);                  // Merging
      x_array[i] = Xg;
   
      MSB = SPI_read(0x2d);                     // Z-axis MSB
      LSB = SPI_read(0x2c);                     // Z-axis LSB
      Zg = (MSB << 8) | (LSB);                  // Merging
      z_array[i] = Zg;
    }
 
    Sort_Signed(x_array, 100);                  // Sorting min to max
    Sort_Signed(z_array, 100);                  // Sorting min to max
 
    x_average = 0;
    z_average = 0;
    for(i = 10; i < 90; i++)                    // removing 10 samples from bottom and 10 from top
    {
      x_average += x_array[i];                  // summing up
      z_average += z_array[i];                  // summing up
    }
 
    x_average /= 80;                            // dividing by the number of samples used
    x_average /= -141;                          // converting to meters per second squared
 
    z_average /= 80;                            // dividing by the number of samples used
    z_average /= -141;                          // converting to meters per second squared
 
    zx_theta = gToDegrees(z_average, x_average);                // getting the degrees between Z and X planes
 
    sprintf(print_buffer, "x: %.0f\tz: %.0f\tZ-X: %.0f", x_average, -z_average, zx_theta);
 
    LCD_string(0,0,print_buffer); 
		USART1_send(print_buffer);
	
	*/
}
