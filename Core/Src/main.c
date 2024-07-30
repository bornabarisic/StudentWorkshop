/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include <stdio.h>

#include "stm32f4xx_hal.h"

#include "measurement.h"
#include "lcd_display.h"
#include "log.h"
#include "flash.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define LCD_WRITE_UPPER_ROW		0, 0 	/* row index, column index */
#define LCD_WRITE_LOWER_ROW 	1, 0 	/* row index, column index */

/* ----------------------------------------------------------------------------------- */
/* Private variables ----------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

static float temperature = 0.0F;
static float humidity 	 = 0.0F;

static char upper_lcd_row_buffer[16] = {0};
static char lower_lcd_row_buffer[16] = {0};

/* ----------------------------------------------------------------------------------- */
/* Private function definitions ------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

/**
  * @brief 	System Clock Configuration
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

	ASSERT(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK);

	/* Initializes the CPU, AHB and APB buses clocks */
	RCC_ClkInitStruct.ClockType 		= RCC_CLOCKTYPE_HCLK|
									  	  RCC_CLOCKTYPE_SYSCLK|
										  RCC_CLOCKTYPE_PCLK1|
										  RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV1;

	ASSERT(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK);
}

/* ----------------------------------------------------------------------------------- */
/* Main ------------------------------------------------------------------------------ */
/* ----------------------------------------------------------------------------------- */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	LoggerInitialize();

	measurement_source_t meas_src = MEASUREMENT_SRC_I2C;

	LOG_INFO("Initializing measurements with %s peripheral\n",
			(meas_src == MEASUREMENT_SRC_ADC) ? "ADC" : "I2C");

	InitializeMeasurements(meas_src);

	LOG_INFO("Initializing LCD display\n");
	InitializeLCD();

	LOG_INFO("Read data from the flash\n");
	FlashReadLogs();

	while (1)
	{
		LOG_DBG("Starting measurements\n");

		temperature = GetTemperature();
		humidity 	= GetHumidity();

		sprintf(upper_lcd_row_buffer, "Temp: %.2f Cel", temperature);
		sprintf(lower_lcd_row_buffer, "Hum:  %.2f Pct", humidity);

		LCDClearScreen();

		LCDWriteData(LCD_WRITE_UPPER_ROW, upper_lcd_row_buffer);
		LCDWriteData(LCD_WRITE_LOWER_ROW, lower_lcd_row_buffer);

		LOG_DBG("Temp: %.2f Cel\n", temperature);
		LOG_DBG("Hum:  %.2f Pct\n", humidity);

		FlashWriteLog(temperature);

		HAL_Delay(2000);
	}
}
