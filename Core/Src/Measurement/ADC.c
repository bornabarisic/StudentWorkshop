/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include "stm32f4xx_hal.h"
#include "ADC.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define POT_GPIO_PIN 			( GPIO_PIN_0 )
#define POT_GPIO_PORT			( GPIOA )

#define ADC_POLL_TIMEOUT_IN_MS	( 1000U )

/* ----------------------------------------------------------------------------------- */
/* Private variables ----------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static ADC_HandleTypeDef hadc1;

/* ----------------------------------------------------------------------------------- */
/* Private function definitions ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

/**
 * @brief 	This function is used to initialize GPIO pins for ADC conversion
 */
static void InitializeADCGPIOPins(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	 /* ADC1 GPIO Configuration: PA1 -> ADC1_IN1 */
	GPIO_InitStruct.Pin  = POT_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(POT_GPIO_PORT, &GPIO_InitStruct);
}

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void InitializeADC(void)
{
	InitializeADCGPIOPins();

	__HAL_RCC_ADC1_CLK_ENABLE();

	/**
	 * Configure the global features of the ADC (Clock, Resolution, Data Alignment
	 * and number of conversion
	 */
	hadc1.Instance 						= ADC1;
	hadc1.Init.ClockPrescaler 			= ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution	 			= ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode 			= DISABLE;
	hadc1.Init.ContinuousConvMode 		= ENABLE;
	hadc1.Init.DiscontinuousConvMode	= DISABLE;
	hadc1.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
	hadc1.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion 			= 1;
	hadc1.Init.DMAContinuousRequests 	= DISABLE;
	hadc1.Init.EOCSelection 			= ADC_EOC_SINGLE_CONV;

	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		while(1);
	}

	/**
	 * Configure for the selected ADC regular channel its corresponding rank in the
	 * sequencer and its sample time.
	 */
	ADC_ChannelConfTypeDef sConfig = {0};

	sConfig.Channel 		= ADC_CHANNEL_0;
	sConfig.Rank 			= 1;
	sConfig.SamplingTime 	= ADC_SAMPLETIME_3CYCLES;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		while(1);
	}
}

/**
  * @brief  This function is used to for reading ADC value.
  * @retval uint16_t raw value from the ADC DR
  */
uint16_t ReadADCData(void)
{
	uint16_t read_value = 0;

	HAL_ADC_Start(&hadc1);

	HAL_ADC_PollForConversion(&hadc1, ADC_POLL_TIMEOUT_IN_MS);

	read_value = HAL_ADC_GetValue(&hadc1);

	HAL_ADC_Stop(&hadc1);

	return read_value;
}
