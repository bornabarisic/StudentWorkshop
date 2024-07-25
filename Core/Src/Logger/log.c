/* ------------------------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

#include <stdarg.h>

#include "stm32f4xx_hal.h"
#include "log.h"

/* ------------------------------------------------------------------------------------*/
/* Defines ----------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

#define LOGGER_GPIO_PORT	( GPIOA )
#define LOGGER_TX_PIN		( GPIO_PIN_2 )
#define LOGGER_RX_PIN		( GPIO_PIN_3 )

/* ------------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

static UART_HandleTypeDef log_uart_handle;

/* ------------------------------------------------------------------------------------*/
/* Private function definitions -------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

static void InitializeLoggerGPIOPins(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin 		= LOGGER_TX_PIN|LOGGER_RX_PIN;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull 		= GPIO_NOPULL;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate 	= GPIO_AF7_USART2;

	HAL_GPIO_Init(LOGGER_GPIO_PORT, &GPIO_InitStruct);
}

/* ------------------------------------------------------------------------------------*/
/* Puclic function definitions --------------------------------------------------------*/
/* ------------------------------------------------------------------------------------*/

void LoggerInitialize(void)
{
	InitializeLoggerGPIOPins();

	__HAL_RCC_USART2_CLK_ENABLE();

	log_uart_handle.Instance 			= USART2;
	log_uart_handle.Init.BaudRate 		= 921600;
	log_uart_handle.Init.WordLength 	= UART_WORDLENGTH_8B;
	log_uart_handle.Init.StopBits 		= UART_STOPBITS_1;
	log_uart_handle.Init.Parity 		= UART_PARITY_EVEN;
	log_uart_handle.Init.Mode 			= UART_MODE_TX;
	log_uart_handle.Init.HwFlowCtl 		= UART_HWCONTROL_NONE;
	log_uart_handle.Init.OverSampling 	= UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&log_uart_handle) != HAL_OK)
	{
		while(1);
	}

	/* Postavljeno ovdje kako bi mi uart ispisao string iako nije na kraju linije
	 * postavljen '\n'
	 *
	 * Pojasnjenje na netu:
	 * 	That specific invocation of setbuf sets stdout to be unbuffered - By default,
	 * 	it's usually line-buffered (whatever gets written to stdout gets flushed to
	 * 	the screen when there's a newline), but after that call there's no buffer and
	 * 	anything you write goes directly to the console.*/
	setbuf(stdout, NULL);
}

/**
 * @brief	This function is used to redirect logger UART output to std output
 */
int _write(int file, char *ptr, int len)
{
	(void)file;

	for (int i=0; i<len; i++)
	{
		while(!(USART2->SR & USART_SR_TXE));

		USART2->DR = (uint8_t)ptr[i];
	}

	return len;
}
