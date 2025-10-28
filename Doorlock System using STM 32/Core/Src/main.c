/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c-lcd.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

char input[5] = {0};         // Stores user input
uint8_t index_input = 0;

char password[5] = "1234";   // default 4-digit password
uint8_t change_mode = 0;     // 0 = normal, 1 = old pass, 2 = new pass, 3 = confirm new pass
char new_pass[5] = {0};      // temporary storage for new password


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
void process_key(char key);
char scan_keypad(void);

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();

    lcd_init();
    lcd_send_cmd(0x01); // Clear LCD
    lcd_put_cur(0,0);
    lcd_send_string(" Enter Pass:");

    while (1)
    {
        char key = scan_keypad();
        if(key != 0)
        {
            process_key(key);
            HAL_Delay(200); // debounce
        }
    }
}

/* Process keypad input */
void process_key(char key)
{
    if(key == '#')   // Enter password change mode
    {
        change_mode = 1;
        index_input = 0;
        memset(input,0,sizeof(input));
        lcd_send_cmd(0x01);
        lcd_put_cur(0,0);
        lcd_send_string(" Old Pass:");
        return;
    }

    if(index_input < 4)
    {
        input[index_input++] = key;
        lcd_put_cur(1,index_input-1);
        lcd_send_string("*"); // mask input
    }

    if(index_input == 4)
    {
        input[4] = '\0';

        if(change_mode == 0) // normal password check
        {
            if(strcmp(input,password) == 0)
            {
                lcd_send_cmd(0x01);
                lcd_put_cur(0,0);
                lcd_send_string(" Access Granted!");
                HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);
                HAL_Delay(3000);
                HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
            }
            else
            {
                lcd_send_cmd(0x01);
                lcd_put_cur(0,0);
                lcd_send_string(" Wrong Pass!");
                HAL_Delay(2000);
            }
        }
        else if(change_mode == 1) // entering old password for change
        {
            if(strcmp(input,password) == 0)
            {
                change_mode = 2;
                index_input = 0;
                memset(input,0,sizeof(input));
                lcd_send_cmd(0x01);
                lcd_put_cur(0,0);
                lcd_send_string(" New Pass:");
            }
            else
            {
                change_mode = 0;
                lcd_send_cmd(0x01);
                lcd_put_cur(0,0);
                lcd_send_string(" Wrong Old Pass");
                HAL_Delay(2000);
            }
        }
        else if(change_mode == 2) // entering new password
        {
            strcpy(new_pass, input);
            change_mode = 3;
            index_input = 0;
            memset(input,0,sizeof(input));
            lcd_send_cmd(0x01);
            lcd_put_cur(0,0);
            lcd_send_string(" Confirm Pass:");
        }
        else if(change_mode == 3) // confirm new password
        {
            if(strcmp(input,new_pass) == 0)
            {
                strcpy(password,new_pass);
                lcd_send_cmd(0x01);
                lcd_put_cur(0,0);
                lcd_send_string(" Pass Changed");
                HAL_Delay(2000);
            }
            else
            {
                lcd_send_cmd(0x01);
                lcd_put_cur(0,0);
                lcd_send_string(" Mismatch");
                HAL_Delay(2000);
            }
            change_mode = 0;
        }

        index_input = 0;
        memset(input,0,sizeof(input));
        if(change_mode == 0)
        {
            lcd_send_cmd(0x01);
            lcd_put_cur(0,0);
            lcd_send_string(" Enter Pass:");
        }
    }
}


/* Simple 4x4 keypad scanning function */
char scan_keypad(void)
{
    const uint16_t row_pins[4] = {Rows_Pin, RowsB1_Pin, RowsB12_Pin, RowsB13_Pin};
    const uint16_t col_pins[4] = {Columns_Pin, ColumnsA2_Pin, ColumnsA3_Pin, ColumnsA4_Pin};
    const char keys[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    for(int r=0; r<4; r++)
    {
        HAL_GPIO_WritePin(GPIOB, row_pins[r], GPIO_PIN_RESET);
        for(int c=0; c<4; c++)
        {
            if(HAL_GPIO_ReadPin(GPIOA, col_pins[c]) == GPIO_PIN_RESET)
            {
                HAL_Delay(20); // debounce delay
                if(HAL_GPIO_ReadPin(GPIOA, col_pins[c]) == GPIO_PIN_RESET) // check again
                {
                    while(HAL_GPIO_ReadPin(GPIOA, col_pins[c]) == GPIO_PIN_RESET); // wait for release
                    HAL_GPIO_WritePin(GPIOB, row_pins[r], GPIO_PIN_SET);
                    return keys[r][c];
                }
            }
        }
        HAL_GPIO_WritePin(GPIOB, row_pins[r], GPIO_PIN_SET);
    }
    return 0; // no key pressed
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { while(1); }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|
                                  RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) { while(1); }
}

/* I2C1 Initialization */
static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if(HAL_I2C_Init(&hi2c1)!=HAL_OK) while(1);
}

/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Relay output
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = Relay_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Relay_GPIO_Port, &GPIO_InitStruct);

    // Rows output
    GPIO_InitStruct.Pin = Rows_Pin|RowsB1_Pin|RowsB12_Pin|RowsB13_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Columns input
    GPIO_InitStruct.Pin = Columns_Pin|ColumnsA2_Pin|ColumnsA3_Pin|ColumnsA4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
