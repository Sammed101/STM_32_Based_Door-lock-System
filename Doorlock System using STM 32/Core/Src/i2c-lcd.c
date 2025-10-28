#include "i2c-lcd.h"

extern I2C_HandleTypeDef hi2c1;
uint8_t LCD_BACKLIGHT = 0x08;

void lcd_send_char(char data)
{
    lcd_send_data(data); // or your existing function to send 1 byte to LCD
}

void lcd_send_cmd(char cmd)
{
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xF0);
    data_l = ((cmd<<4) & 0xF0);
    data_t[0] = data_u|0x0C; // EN=1, RS=0
    data_t[1] = data_u|0x08; // EN=0, RS=0
    data_t[2] = data_l|0x0C;
    data_t[3] = data_l|0x08;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, data_t, 4, 100);
}

void lcd_send_data(char data)
{
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xF0);
    data_l = ((data<<4) & 0xF0);
    data_t[0] = data_u|0x0D; // EN=1, RS=1
    data_t[1] = data_u|0x09; // EN=0, RS=1
    data_t[2] = data_l|0x0D;
    data_t[3] = data_l|0x09;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDRESS, data_t, 4, 100);
}

void lcd_init(void)
{
    HAL_Delay(50);
    lcd_send_cmd(0x33);
    lcd_send_cmd(0x32);
    lcd_send_cmd(0x28);
    lcd_send_cmd(0x0C);
    lcd_send_cmd(0x06);
    lcd_send_cmd(0x01);
    HAL_Delay(2);
}

void lcd_send_string(char *str)
{
    while(*str) lcd_send_data(*str++);
}

void lcd_put_cur(int row, int col)
{
    col = (row == 0 ? 0x80 + col : 0xC0 + col);
    lcd_send_cmd(col);
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);
    HAL_Delay(2);
}
