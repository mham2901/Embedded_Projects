#include <stdlib.h>

#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#define LCD_ADDR (0x4E)
#define INST_REG (0x00)
#define DATA_REG (0x40)


void GPIO_Init_(void);
void TIM2_Init(void);
void TIM4_ms_Delay(uint32_t delay);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Write(uint8_t var);
void I2C_Send_Addr(uint8_t Addr);
void I2C_Stop(void);
void LCD_Init(void);
void LCD_Write_Cmd(uint8_t Device_Addr, uint8_t Slave_Reg_Addr, uint8_t data);
void LCD_Write_Data(uint8_t Device_Addr, uint8_t Slave_Reg_Addr, uint8_t data);
void LCD_Cursor(int r, int c);
void TIM4_ms_Delay_2(uint16_t delay);
void config(void);
void config(void); 
void tache1_FreeRTOS(void *params) ; 
void tache2_FreeRTOS(void *params) ;
void tache3_give(void *params) ; 


