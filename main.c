#include "main.h"




uint16_t mysensordata=0;
char strsensordata[30];

int main()
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
	 /* Configure the system clock */
  SystemClock_Config();
	
	/* Initialize all configured peripherals */
  Oscillator_IO_Init();
	MX_Probe_IO_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
	
	Lcd_MX_GPIO_Init();
	Lcd_Init(LCD_NUM_OF_LINES, LCD_NUM_OF_CHARS);					//4 lines, 20 characters per line
	Lcd_DisplayString((char *)"WELCOME\n");
	
	//Lcd_GoToLine(2);	//go to line 2
	Lcd_DisplayString((char *)"VIBUNANDAN,RISHI \n");
	//Lcd_GoToLine(3);	//go to line 3
	Lcd_DisplayString((char *)"AADHYA,ANVIKSHA, ");
	
	HAL_Delay(5000);
	Lcd_Clear();  
	Lcd_DisplayString((char *)"POTENTIOMETER READING");
	Lcd_GoToLine(3);	//go to line 3
			
	HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_Base_Start_IT(&htim5);
	
	while (1)
  {
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 50);
	  mysensordata=HAL_ADC_GetValue(&hadc1);

	  sprintf(strsensordata,"VALUE %hu",mysensordata);
	  Lcd_DisplayString((char *)strsensordata);
	  Lcd_GoToLine(3);	//go to line 3
	  HAL_Delay(1000);

  }

}