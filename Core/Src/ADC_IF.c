/*
 ******************************************************************************
 * @file    ADC.h
 * @brief   A/D Converter Interface.
 ******************************************************************************
*/

// Includes
#include "math.h"
#include "stm32f4xx_hal.h"
#include "ADC_IF.h"

// Private variables
static uint16_t readVal;
static double delay_factor;
ADC_HandleTypeDef hadc1;

// Global function prototypes
void Error_Handler(void);

// Function definitions

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function makes the LED blink with the frequency set by the potentiometer using ADC.
  * @retval None
  */
void LED_control_with_ADC (void)
{
	HAL_ADC_Start(&hadc1);

	HAL_ADC_PollForConversion(&hadc1, 1000);
	readVal = HAL_ADC_GetValue(&hadc1);

	HAL_ADC_Stop(&hadc1);

	// log e (x)
	//	  delay_factor = 0.01 + 0.119*log10((1 + (double)readVal));
	// log 10 (x)
	//	  delay_factor = 0.01 + 0.27406*log10((1 + (double)readVal));
	// exponential
	delay_factor = 0.1 * pow(1.0005623126, (double)readVal);
	HAL_Delay(1000*delay_factor);

	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
