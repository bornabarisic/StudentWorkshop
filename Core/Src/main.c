/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include <math.h>

#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "ADC.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define ADC_DATA_RESOLUTION  	( 4096U ) 	/* 12 bit */
#define LED_BLINK_REF_TIME		( 1000U ) 	/* Reference time, 1ms */

/* ----------------------------------------------------------------------------------- */
/* Private function definitions ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

/**
 * @brief	This function is calculating delay in milliseconds with linear
 * 			characteristics
 *
 * @retval	int delay
 */
static int CalculateDelayLinChar(uint16_t adc_data)
{
	/* Calculate the delay factor */
	float delay_factor = (float)adc_data/(float)ADC_DATA_RESOLUTION;

	/* Calculate the delay in milliseconds */
	int delay = (int)(LED_BLINK_REF_TIME * delay_factor);

	return delay;
}

/**
 * @brief	This function is calculating delay in milliseconds with exponential
 * 			characteristics
 *
 * @retval	int delay
 */
static int CalculateDelayExpChar(uint16_t adc_data)
{
	/* Calculate the delay factor */
	float delay_factor = 0.1 * pow(1.0005623126, (float)adc_data);

	/* Calculate the delay in milliseconds */
	int delay = (int)(LED_BLINK_REF_TIME * delay_factor);

	return delay;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/* Configure the main internal regulator output voltage */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/**
	 * Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType 		= RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState 				= RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue 	= RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState 			= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource 		= RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM 				= 16;
	RCC_OscInitStruct.PLL.PLLN 				= 336;
	RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ 				= 2;
	RCC_OscInitStruct.PLL.PLLR 				= 2;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		while(1);
	}

	/* Initializes the CPU, AHB and APB buses clocks */
	RCC_ClkInitStruct.ClockType 		= RCC_CLOCKTYPE_HCLK|
									  	  RCC_CLOCKTYPE_SYSCLK|
										  RCC_CLOCKTYPE_PCLK1|
										  RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		while(1);
	}
}

/* ----------------------------------------------------------------------------------- */
/* Main ------------------------------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

/**
  * @brief  	The application entry point.
  * @retval 	int
  */
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	InitializeBoardSupport();

	InitializeADC();

	while (1)
	{
		/* Read the value of the ADC conversion */
		uint16_t adc_value = ReadADCData();

		/* Calculate delay with linear characteristics */
		int delay_ms = CalculateDelayLinChar(adc_value);

		/* Calculate delay with Logarithimc characteristics */
//		int delay_ms = CalculateDelayExpChar(adc_value);

		/* Send the  */
		BlinkLEDWithConstantDelay(delay_ms);
	}
}
