#include "battery.h"

ADC_HandleTypeDef hadc1;

void Battery_ADC_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	__HAL_RCC_ADC1_CLK_ENABLE();
	HAL_ADC_Init(&hadc1);
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	System_GPIO_Init(BATT_ADC_INPUT_GPIO_GROUP, BATT_ADC_INPUT_GPIO_ID,
		GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

	HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);
}

struct BatteryData batteryData;

void Battery_Init(void)
{
	Battery_ADC_Init();
	HAL_ADC_Start(&hadc1);
	Battery_UpdateData();
}

void Battery_UpdateData(void)
{
	HAL_ADC_PollForConversion(&hadc1, BATT_ADC_TIMEOUT);
	if (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
	{
		batteryData.batteryVoltageRaw = HAL_ADC_GetValue(&hadc1);
		batteryData.batteryVoltage = 
			(batteryData.batteryVoltageRaw * BATT_ADC_REF / BATT_ADC_RESOLU) * BATT_K_VOL;
		batteryData.batteryPercent = 
			(int32_t) (batteryData.batteryVoltage * BATT_K_VOL - BATT_B_PERCENT);
		batteryData.batteryPercent = batteryData.batteryPercent > 100 ? 100 :
			(batteryData.batteryPercent < 0 ? 0 : batteryData.batteryPercent);
		batteryData.batteryState = 
			batteryData.batteryPercent > BATT_LOW_PERCENT ? BATT_STATE_NORMAL : BATT_STATE_LOW;
	}
}

osThreadId_t batterySamplingTaskHandle;
const osThreadAttr_t batterySamplingTask_attributes = {
	.name = "batterySamplingTask",
	.priority = (osPriority_t) osPriorityLow,
	.stack_size = 512 * 4
};

osMutexId_t batteryDataMutexHandle;
const osMutexAttr_t batteryDataMutex_attributes = {
	.name = "batteryDataMutex"
};

void Battery_RunTask(void)
{
	batteryDataMutexHandle = 
		osMutexNew(&batteryDataMutex_attributes);
	batterySamplingTaskHandle = 
		osThreadNew(Battery_Task, NULL, &batterySamplingTask_attributes);
}

void Battery_Task(void *argument)
{
	while(1)
	{
		osMutexAcquire(batteryDataMutexHandle, osWaitForever);
		Battery_UpdateData();
		osMutexRelease(batteryDataMutexHandle);
		osDelay(BATT_SAMPLE_PERIOD);
	}
}
