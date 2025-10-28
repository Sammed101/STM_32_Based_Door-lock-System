#ifndef INC_I2C_LCD_H_
#define INC_I2C_LCD_H_

#include "stm32f1xx_hal.h"

#define LCD_ADDRESS (0x27<<1) // change according to your module

void lcd_init(void);
void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void lcd_send_char(char data);
void lcd_send_string(char *str);
void lcd_put_cur(int row, int col);
void lcd_clear(void);

#endif /* INC_I2C_LCD_H_ */
