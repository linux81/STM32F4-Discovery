

void set_page(char page);
void set_line(char line);
void send_cmd(uint16_t number1);
void send_grdata(uint16_t number2);
void OledBmp(char *);
void LCD_init(void);
void set_contrast(char contr);
void norm_disp(void);
void rev_disp(void);
void disp_on(void);
void disp_off(void);