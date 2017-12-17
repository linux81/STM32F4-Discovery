void HD44780_Cmd(uint8_t cmd);
void HD44780_Data(uint8_t data);

void LCD_init(void);
void send_data_4bit(uint8_t data, uint8_t RS);
void LCD_clear(void);
void LCD_string(char *str);
void PCF8574_write (char Dat);
void set_DRAM_adr(char adr);
void LCD_Text(unsigned char line, char* string);
