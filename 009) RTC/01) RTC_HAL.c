#include "stm32f4xx_hal.h"

void SystemClock_Config(void);
void LED_Config(void);
void RTC_Config(void);

RTC_HandleTypeDef myRtcHandle;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

void readdate(void);
void readtime(void);
unsigned int bcd2decimal(unsigned int num);
unsigned int  decimal2bcd(unsigned int num);
unsigned long  sec,min,hour,day,date,month,year;
uint32_t time,calender;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	RTC_Config();

	//1) Set time 
	sTime.Hours = 23;
	sTime.Minutes = 59;
	sTime.Seconds = 45;
	HAL_RTC_SetTime(&myRtcHandle, &sTime, RTC_FORMAT_BIN);
	//2) Set date 
	sDate.Date = 31;
	sDate.Month = RTC_MONTH_DECEMBER;
	sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
	sDate.Year = 21;
	HAL_RTC_SetDate(&myRtcHandle, &sDate, RTC_FORMAT_BIN);
	
	
	
	while(1)
	{
		readtime();
		readdate();
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void RTC_Config(void)
{
	__HAL_RCC_RTC_ENABLE();
	myRtcHandle.Instance = RTC;
	myRtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	myRtcHandle.Init.AsynchPrediv = 127;
	myRtcHandle.Init.SynchPrediv = 255;
	myRtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	HAL_RTC_Init(&myRtcHandle);
}

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

void readtime(void){
	time = RTC->TR;
	hour = bcd2decimal((time & (0x3FUL<<16)));
	min = bcd2decimal((time & (0x7FUL<<8)));
	sec = bcd2decimal((time & (0x7FUL<<0)));
}

void readdate(void){
	calender =RTC->DR;
	year = bcd2decimal((calender & (0xFFUL<<16)));
	day = bcd2decimal((calender & (0x7UL<<13)));
	month = bcd2decimal((calender & (0x1FUL<<8)));
	date = bcd2decimal((calender & (0x3FUL<<0)));
}
unsigned int  decimal2bcd(unsigned int num){
	return ((num/10 * 16) + (num % 10));
}
unsigned int bcd2decimal(unsigned int num){
	return ((num/16 * 10) + (num % 16));
}
