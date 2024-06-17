/* ----------------------------------------------------------------------------------- */
/* Includes -------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#include "stm32f4xx_hal.h"

#include "gpio.h"

/* ----------------------------------------------------------------------------------- */
/* Defines --------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

#define LD2_Pin 		GPIO_PIN_5
#define LD2_GPIO_Port 	GPIOA

#define B1_Pin 			GPIO_PIN_13
#define B1_GPIO_Port 	GPIOC

/* ----------------------------------------------------------------------------------- */
/* Public function definitions ------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/**
 * @brief 	GPIO Initialization Function
 * @param 	None
 * @retval	None
 */
void InitializeBoardSupport(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin 	= B1_Pin;
	GPIO_InitStruct.Mode 	= GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LD2_Pin */
	GPIO_InitStruct.Pin 	= LD2_Pin;
	GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 	This function is used to manage blinking of a green LED
 * @note 	Mode of operation: On each blue push button press, green LED
 * 			changes its state.
 */
void BlinkLEDWithPushButton(void)
{
	/* Check if the blue push button is pressed */
	if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
	{
		/* Wait until user releases blue push button */
		while(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin));

		/* Change the state of the green LED */
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}
}

/**
 * @brief	This function is used to manage blinking of a green LED
 * @note 	Mode of operation: Green LED changes its state every time
 * 			interval determined by the time declared in the delay_ms
 * 			variable.
 */
void BlinkLEDWithConstantDelay(int delay_ms)
{
	/* Change the state of the green LED */
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	/* Wait until time interval elapses */
	HAL_Delay(delay_ms);
}
