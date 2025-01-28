#include "Configuration.h"
#include "math.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

xSemaphoreHandle SEM1;
int j=1;
int k=1;

//============================================= Tasks ============================================

//                      ===============================================================================
//                                              Task_1
//                      =============================================================================== 
void tache1_FreeRTOS(void *params) {
	
	do {
		
		j=1-j;
		if(j){ 
				GPIO_Init_();
	I2C_Init();
	LCD_Init();
	LCD_Cursor(0,0); // Set the cursor at the first row and first column
	LCD_Write_Data(LCD_ADDR,DATA_REG, 0x84); // Character Generation ROM Pattern for 'H'
	LCD_Write_Data(LCD_ADDR,DATA_REG, 0x54); // Character Generation ROM Pattern for 'E'
	LCD_Write_Data(LCD_ADDR,DATA_REG, 0xC4); // Character Generation ROM Pattern for 'L'
	LCD_Write_Data(LCD_ADDR,DATA_REG, 0xC4); // Character Generation ROM Pattern for 'L'
	LCD_Write_Data(LCD_ADDR,DATA_REG, 0xF4); // Character Generation ROM Pattern for '0'
	TIM4_ms_Delay(1000); // Delay of 1s
	while(1){ // Dead Loop
	}
		}
	

		vTaskDelay(1000);
	xSemaphoreTake(SEM1, (portMAX_DELAY));	
	} while(1);
}
//                      ===============================================================================
//                                                 Task_2
//                      =============================================================================== 
void tache2_FreeRTOS(void *params) {
	
	do {
		
   k=1-k;
		if (k) {
			
			LCD_Write_Data(LCD_ADDR,DATA_REG, 0x75); 
	TIM4_ms_Delay(1000);
	do {

if(TIM2->CCR1 < 2500){
			TIM2->CCR1 = TIM2->CCR1 + 5;
			TIM4_ms_Delay(50);		}
		else{
			TIM2->CCR1 = 00;
			TIM4_ms_Delay(50);

} 
}while(1);
		xSemaphoreTake(SEM1, (portMAX_DELAY));	
//		vTaskDelay(500);
	}	
	} while(1);

}
//                      ===============================================================================
//                                           Semaphore Give with the user button
//                      =============================================================================== 
void tache3_give(void *params) {
	
	do {
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) {
			xSemaphoreGive( SEM1);
		}
   vTaskDelay(300);
		
	} while(1);
}

//                      ===============================================================================
//                      ===============================================================================
//                                          Main Program - Begin
//                      =============================================================================== 
//                      ===============================================================================

int main(void) {
config();
vSemaphoreCreateBinary(SEM1);
xTaskCreate (tache1_FreeRTOS,"tache1",128,NULL,2,NULL) ;
xTaskCreate (tache2_FreeRTOS,"tache2",128,NULL,3,NULL) ;
xTaskCreate (tache3_give,"tache3",128,NULL,2,NULL) ;
vTaskStartScheduler ( ) ;
}
//                      ===============================================================================
//                      ===============================================================================
//                                           Main Program - End
//                      =============================================================================== 
//                      ===============================================================================
