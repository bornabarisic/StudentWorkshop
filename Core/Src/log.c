#include <stdarg.h>

#include "stm32f4xx_hal.h"
#include "log.h"

// Function prototypes
void log_uart_init_pins(UART_HandleTypeDef* huart);

UART_HandleTypeDef log_uart_handle;

void log_uart_init(void)
{
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
