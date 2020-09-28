#include "motor.h"

TIM_HandleTypeDef htim1;

void Motor_Timer_Init(void)
{
	// TIM1
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 8400 - 1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	__HAL_RCC_TIM1_CLK_ENABLE();
	HAL_TIM_PWM_Init(&htim1);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

	// GPIO
	System_GPIO_Alt_Init(MOTOR_R_PWM_GROUP, MOTOR_R_PWM_GPIO,
		GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF1_TIM1);
	System_GPIO_Alt_Init(MOTOR_L_PWM_GROUP, MOTOR_L_PWM_GPIO,
		GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF1_TIM1);
}

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

void Motor_Encoder_Init(void)
{
	// TIM2
	TIM_Encoder_InitTypeDef sConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 0;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 0;
	__HAL_RCC_TIM2_CLK_ENABLE();
	HAL_TIM_Encoder_Init(&htim2, &sConfig);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	
	// TIM3
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	__HAL_RCC_TIM3_CLK_ENABLE();
	HAL_TIM_Encoder_Init(&htim3, &sConfig);
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

	// GPIO
	System_GPIO_Alt_Init(MOTOR_R_ENCA_GROUP, MOTOR_R_ENCA_GPIO,
		GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF1_TIM2);
	System_GPIO_Alt_Init(MOTOR_R_ENCB_GROUP, MOTOR_R_ENCB_GPIO,
		GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF1_TIM2);
	System_GPIO_Alt_Init(MOTOR_L_ENCA_GROUP, MOTOR_L_ENCA_GPIO,
		GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF2_TIM3);
	System_GPIO_Alt_Init(MOTOR_L_ENCB_GROUP, MOTOR_L_ENCB_GPIO,
		GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF2_TIM3);
}

RobotMotorData robotMotorData;

void Motor_Init(void)
{
	Motor_Timer_Init();
	Motor_Encoder_Init();
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

	System_GPIO_Init(MOTOR_R_A_GROUP, MOTOR_R_A_GPIO, 
		GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
	System_GPIO_Init(MOTOR_R_B_GROUP, MOTOR_R_B_GPIO, 
		GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
	System_GPIO_Init(MOTOR_L_A_GROUP, MOTOR_L_A_GPIO, 
		GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);
	System_GPIO_Init(MOTOR_L_B_GROUP, MOTOR_L_B_GPIO, 
		GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);

	for (int i = 0; i < MOTOR_NUMBER; i++)
	{
		robotMotorData[i].currentPWM = 0;
		robotMotorData[i].currentSpeedRPM = 0;
		robotMotorData[i].currentSpeedPPS = 0;
		robotMotorData[i].encoderCountAbs = 0;
		robotMotorData[i].encoderCountPeriod = 0;
		robotMotorData[i].lastError = 0;
		robotMotorData[i].targetSpeedPPS = 0;
	}
}

void Motor_SetPWM(uint8_t _index, int32_t _value)
{
	uint8_t _rev = 0;
	_value = _value > MOTOR_MAX_PWM ? MOTOR_MAX_PWM : 
		(_value < -1 * MOTOR_MAX_PWM ? -1 * MOTOR_MAX_PWM : _value);
	if (_value < 0)
	{
		_value = -1 * _value;
		_rev = 1;
	}
	TIM_OC_InitTypeDef sConfigOC = {0};
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = _value;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (_index == MOTOR_RIGHT)
	{
		if (_value == 0)
		{
			System_GPIO_Write(MOTOR_R_A_GROUP, MOTOR_R_A_GPIO, LOW);
			System_GPIO_Write(MOTOR_R_B_GROUP, MOTOR_R_B_GPIO, LOW);
		}
		else if (_rev == 1)
		{
			System_GPIO_Write(MOTOR_R_A_GROUP, MOTOR_R_A_GPIO, HIGH);
			System_GPIO_Write(MOTOR_R_B_GROUP, MOTOR_R_B_GPIO, LOW);
		}
		else
		{
			System_GPIO_Write(MOTOR_R_A_GROUP, MOTOR_R_A_GPIO, LOW);
			System_GPIO_Write(MOTOR_R_B_GROUP, MOTOR_R_B_GPIO, HIGH);
		}
		HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	}
	else if (_index == MOTOR_LEFT)
	{
		if (_value == 0)
		{
			System_GPIO_Write(MOTOR_L_A_GROUP, MOTOR_L_A_GPIO, LOW);
			System_GPIO_Write(MOTOR_L_B_GROUP, MOTOR_L_B_GPIO, LOW);
		}
		else if (_rev == 1)
		{
			System_GPIO_Write(MOTOR_L_A_GROUP, MOTOR_L_A_GPIO, LOW);
			System_GPIO_Write(MOTOR_L_B_GROUP, MOTOR_L_B_GPIO, HIGH);
		}
		else
		{
			System_GPIO_Write(MOTOR_L_A_GROUP, MOTOR_L_A_GPIO, HIGH);
			System_GPIO_Write(MOTOR_L_B_GROUP, MOTOR_L_B_GPIO, LOW);
		}
		HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	}
}

void Motor_ClearEncoder(uint8_t _index)
{
	if (_index == MOTOR_RIGHT)
		__HAL_TIM_SET_COUNTER(&htim2, 0);
	else __HAL_TIM_SET_COUNTER(&htim3, 0);
}

short Motor_ReadEncoder(uint8_t _index)
{
	if (_index == MOTOR_RIGHT)
		return (short) __HAL_TIM_GET_COUNTER(&htim2);
	else return -1 * (short) __HAL_TIM_GET_COUNTER(&htim3);
}

osTimerId_t motorControlTaskHandle;
const osTimerAttr_t motorControlTask_attributes = {
	.name = "motorControlTaskTimer"
};

osMutexId_t motorDataMutexHandle;
const osMutexAttr_t motorDataMutexHandle_attributes = {
	.name = "motorDataMutex"
};

void Motor_RunTask(void)
{
	motorDataMutexHandle = 
		osMutexNew(&motorDataMutexHandle_attributes);
	motorControlTaskHandle = 
		osTimerNew(Motor_Task, osTimerOnce, NULL, &motorControlTask_attributes);
	osTimerStart(motorControlTaskHandle, MOTOR_CONTROL_PERIOD);
}

void Motor_SetSpeedPPS(uint8_t _index, int32_t _value)
{
	osMutexAcquire(motorDataMutexHandle, osWaitForever);

	robotMotorData[_index].targetSpeedPPS = _value;

	osMutexRelease(motorDataMutexHandle);
}

void Motor_Task(void *argument)
{
	osMutexAcquire(motorDataMutexHandle, osWaitForever);

	for (int i = 0; i < MOTOR_NUMBER; i++)
	{
		// Update Data
		robotMotorData[i].currentSpeedPPS = 
			(int32_t) (Motor_ReadEncoder(i) * 1000.0f / MOTOR_CONTROL_PERIOD);
		robotMotorData[i].currentSpeedRPM = 
			(int32_t) (robotMotorData[i].currentSpeedPPS * 60.0f / MOTOR_PPR);
		robotMotorData[i].encoderCountAbs += abs(Motor_ReadEncoder(i));
		robotMotorData[i].encoderCountPeriod += Motor_ReadEncoder(i);
		Motor_ClearEncoder(i);

		// Calc PWM
		int32_t currentError = 
			robotMotorData[i].targetSpeedPPS - robotMotorData[i].currentSpeedPPS;
		robotMotorData[i].currentPWM += (int32_t) (
			MOTOR_PID_KP * (currentError - robotMotorData[i].lastError) +
			MOTOR_PID_KI * currentError);
		robotMotorData[i].lastError = currentError;
		
		// Apply PWM
		robotMotorData[i].currentPWM = 
			robotMotorData[i].currentPWM > MOTOR_MAX_PWM ? MOTOR_MAX_PWM : 
			(robotMotorData[i].currentPWM < -1 * MOTOR_MAX_PWM ? 
			-1 * MOTOR_MAX_PWM : robotMotorData[i].currentPWM);
		Motor_SetPWM(i, robotMotorData[i].currentPWM);
	}

	osMutexRelease(motorDataMutexHandle);

	osTimerStart(motorControlTaskHandle, MOTOR_CONTROL_PERIOD);
}
