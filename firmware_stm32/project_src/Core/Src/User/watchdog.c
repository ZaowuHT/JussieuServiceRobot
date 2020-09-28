#include "watchdog.h"

IWDG_HandleTypeDef hiwdg;

void Watchdog_Init(void)
{
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
	hiwdg.Init.Reload = WATCHDOG_RESET_DURATION;
	HAL_IWDG_Init(&hiwdg);
}

void Watchdog_Feed(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
