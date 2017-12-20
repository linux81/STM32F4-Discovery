#include "stm32f4xx.h"
#include <oled_SH1106.h>
#include <delay.h>




void LCD_init(void){
uint8_t i;
	uint8_t buff_init[]=
{
    0x80,  //control byte
    0xae,//Display OFF
    0x00,//Low Column
    0x10,//High Column
    0xB0,//Page 
    0x40,//Start line
    0xA1,//remap 
    0xDA,//com pins 
    0x12,
    0xD3, //display offset
    0x00,//NO offset
    0xc0,//scan direction
    0xc8,
    0xA6,//normal display
    0xA4,//display ON 
    0x81,//set contrast
    0x50,//contrast DATA
    0xa8,//multiplex ratio 
    0x3f,//1/64 duty 
    0xD5,//Display clock divide
    0x80,
    0xd9,//precharge period
    0xF1,
    0xDB,//VCOM deselect
    0x40,
    0x8d,//charge pump 
    0x14,
    0xAF,//display ON
};
	
	Delay(10);

GPIOB->BSRRH |=GPIO_BSRR_BS_1; // PB1 reset LCD
		
	
Delay(500);

GPIOB->BSRRL |= GPIO_BSRR_BS_1;
	

	
	
for(i=0;i<29;i++){

send_cmd(buff_init[i]);
}


}



void send_cmd(uint16_t number1){
uint16_t Data;
	

	
GPIOB->BSRRH = GPIO_BSRR_BS_0; // COMMAND MODE PA0

//Delay(1);

	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = number1;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	Data = SPI1->DR;

};


void send_grdata(uint16_t number2){
	uint16_t Data;
	
GPIOB->BSRRL = GPIO_BSRR_BS_0;
	
// GRAPHIC MODE
	
//Delay(1);

	while (!(SPI1->SR & SPI_SR_TXE));	//Wait if TXE cleared, Tx FIFO is full.
	SPI1->DR = number2;
	while (!(SPI1->SR & SPI_SR_RXNE));	//Wait if RNE cleared, Rx FIFO is empty.
	Data = SPI1->DR;
	
//Delay(1);

};


void set_page(char page){
	
	// page number from 0 to 6
uint16_t a;
	
	a=0xb0|page;
	
	
send_cmd(a);
	
}


void set_line(char line){
uint16_t a;
	// line number from 0 to 63
	a=0x40|line;
	
	
send_cmd(a);
	
}

void set_col(char col){
	
// col number from 0 to 131
uint16_t a,b;
	
	a=0x10|(col&0xf0)>>4;
	b=col&0xf;
	
	
send_cmd(a);
send_cmd(b);
	
}


void OledBmp(char *bmp){
	
	
uint16_t i,j,k;
set_col(0);
set_line(0);
set_page(0);
k=0;
for(j=0;j<8;j++)
{
set_page(j);
set_col(0);
for (i=0; i<128; i++) send_grdata(bmp[k++]);
}
};

void set_contrast(char contr){
send_cmd(0x81);
send_cmd(contr);
}

void norm_disp(void){
send_cmd(0xa6);

}

void rev_disp(void){
send_cmd(0xa7);

}

void disp_on(void){
send_cmd(0xaf);

}

void disp_off(void){
send_cmd(0xae);

}