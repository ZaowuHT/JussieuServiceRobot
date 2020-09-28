#include "system.h"

// 设置系统时钟
void System_Clock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	__HAL_RCC_SYSCFG_CLK_ENABLE();
	HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}

// 设置GPIO时钟
void System_GPIO_CLK_Config(GPIO_TypeDef *_group)
{
	if (_group == GPIOA)
	{
		if (!__HAL_RCC_GPIOA_IS_CLK_ENABLED())
			__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (_group == GPIOB)
	{
		if (!__HAL_RCC_GPIOB_IS_CLK_ENABLED())
			__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (_group == GPIOC)
	{
		if (!__HAL_RCC_GPIOC_IS_CLK_ENABLED())
			__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (_group == GPIOD)
	{
		if (!__HAL_RCC_GPIOD_IS_CLK_ENABLED())
			__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if (_group == GPIOE)
	{
		if (!__HAL_RCC_GPIOE_IS_CLK_ENABLED())
			__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if (_group == GPIOF)
	{
		if (!__HAL_RCC_GPIOF_IS_CLK_ENABLED())
			__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if (_group == GPIOG)
	{
		if (!__HAL_RCC_GPIOG_IS_CLK_ENABLED())
			__HAL_RCC_GPIOG_CLK_ENABLE();
	}
	else if (_group == GPIOH)
	{
		if (!__HAL_RCC_GPIOH_IS_CLK_ENABLED())
			__HAL_RCC_GPIOH_CLK_ENABLE();
	}
}

// 初始化普通GPIO
void System_GPIO_Init(GPIO_TypeDef *_group, uint32_t _pin, uint32_t _mode, uint32_t _pull, uint32_t _speed)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	System_GPIO_CLK_Config(_group);

	GPIO_InitStruct.Pin = _pin;
	GPIO_InitStruct.Mode = _mode;
	GPIO_InitStruct.Pull = _pull;
	GPIO_InitStruct.Speed = _speed;

	HAL_GPIO_Init(_group, &GPIO_InitStruct);

	if (_mode == GPIO_MODE_OUTPUT_PP || _mode == GPIO_MODE_OUTPUT_OD)
		System_GPIO_Write(_group, _pin, LOW);
}

// 初始化复用GPIO
void System_GPIO_Alt_Init(GPIO_TypeDef *_group, uint32_t _pin, uint32_t _mode, uint32_t _pull, uint32_t _speed, uint32_t _alt)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	System_GPIO_CLK_Config(_group);

	GPIO_InitStruct.Pin = _pin;
	GPIO_InitStruct.Mode = _mode;
	GPIO_InitStruct.Pull = _pull;
	GPIO_InitStruct.Speed = _speed;
	GPIO_InitStruct.Alternate = _alt;

	HAL_GPIO_Init(_group, &GPIO_InitStruct);

	if (_mode == GPIO_MODE_OUTPUT_PP || _mode == GPIO_MODE_OUTPUT_OD)
		System_GPIO_Write(_group, _pin, LOW);
}

// GPIO写入电平
void System_GPIO_Write(GPIO_TypeDef *_group, uint32_t _pin, uint8_t _level)
{
	HAL_GPIO_WritePin(_group, _pin, (GPIO_PinState)_level);
}

// GPIO读取电平
uint8_t System_GPIO_Read(GPIO_TypeDef *_group, uint32_t _pin)
{
	return (uint8_t)HAL_GPIO_ReadPin(_group, _pin);
}
