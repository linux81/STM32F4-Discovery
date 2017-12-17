#include "stm32f4xx.h"
#include "delay.h"
#include "lcd.h"


void send_data_4bit(uint8_t data, uint8_t RS){
	
uint16_t dat;
//dat = data |backlight|EN|RS 
  dat=(data<<4)|0X08|0x4|RS;

	PCF8574_write(dat);
	PCF8574_write(dat&~0x04); // latch data

	
};


void HD44780_Cmd(uint8_t cmd) {
	//HD44780_RS_LOW;
	
	send_data_4bit((cmd >> 4),0);			//High nibble
	send_data_4bit((cmd & 0x0F),0);			//Low nibble
};

void HD44780_Data(uint8_t data) {
	//HD44780_RS_HIGH;
	
	send_data_4bit((data >> 4),1);			//High nibble
	send_data_4bit((data & 0x0F),1);			//Low nibble
};






void LCD_string(char *str) {

 
	
	while (*str != '\0') {                // send all characters in a string
    HD44780_Data (*str++);
  }
};





void LCD_init (void) {
	
RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOBEN;
RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
GPIOB->AFR[1] |= (4<<8)|(4<<12);
GPIOB->OTYPER |= GPIO_OTYPER_OT_10|GPIO_OTYPER_OT_11;
	
	// PB10 I2C2 SCL, PB11 I2C2 SDA 
GPIOB->MODER  |= GPIO_MODER_MODER10_1|GPIO_MODER_MODER11_1;  //  PA5,PA6, PA7 => AF mode

  I2C2->CR2     |= 0x0008;           // clock == 8MHz!    
  I2C2->CCR     |= 0x0040;           // clock control register (270kHz) 
  I2C2->TRISE   |= 0x0009;           // rise time register 
  I2C2->CR1     |= 0x0001;           // I2C3 enable
	
	
	Delay(100);
	HD44780_Cmd(0x03);        Delay(5);
  HD44780_Cmd(0x03);        Delay(5);
  HD44780_Cmd(0x03);        Delay(5);
  HD44780_Cmd(0x02);        Delay(5);

  HD44780_Cmd(0x0c);
  HD44780_Cmd(0x01);
  HD44780_Cmd(0x06);
  Delay(5);
	


};


void LCD_clear (void){

HD44780_Cmd(0x01);
Delay(5);
};








void PCF8574_write(char Dat)    {
  int Status2;
  I2C2->CR1         |= I2C_CR1_START;       // send START bit 
  while (!(I2C2->SR1 & I2C_SR1_SB)) {};  // wait for START condition (SB=1)  
  I2C2->DR           = 0x4e;         // slave address PCF8574
  while (!(I2C2->SR1 & I2C_SR1_ADDR)) {};  // wait for ADDRESS sent (ADDR=1) 
	
  Status2        = I2C2->SR2;    // read status to clear flag 
  I2C2->DR           = Dat;          // Dat -> DR & write 
  while (!(I2C2->SR1 & I2C_SR1_TXE)) {};  // wait for DR empty (TxE)  
  while (!(I2C2->SR1 & I2C_SR1_BTF)) {};  // wait for Byte sent (BTF) 
  I2C2->CR1         |= I2C_CR1_STOP;       // send STOP bit 
} 


void set_DRAM_adr(char adr){
HD44780_Cmd(adr|0X80);

}

void LCD_Text(unsigned char line, char* string)
{  
    unsigned char addr; 
    if(line==1) // 
        addr=0x00; //The first line address 00h-13h for display 2004 for display 2x16 00h-Fh
    else if(line==2) // 
        addr=0x40; //The second line address 40h - 53h for display 2004 display 2x16 40h-4Fh
		    if(line==3) 
        addr=0x14; //The third line address 14h- 27h for display 2004
    else if(line==4) 
        addr=0x54; // fourth line address 54h - 67h for display 2004


set_DRAM_adr(addr);
		
LCD_string(string);
}